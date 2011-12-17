// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <qtextstream.h> 
#include <qfile.h>
#include <qfileinfo.h>

#include "UmlOperation.h"
#include "UmlSettings.h"
#include "PhpSettings.h"
#include "UmlCom.h"
#include "UmlClass.h"
#include "UmlRelation.h"
#include "util.h"

// to manage preserved bodies, the key is the id under bouml
QIntDict<char> UmlOperation::bodies(127);
const char * BodyPrefix = "// Bouml preserved body begin ";
const char * BodyPostfix = "// Bouml preserved body end ";
const int BodyPrefixLength = 30;
const int BodyPostfixLength = 28;

static bool generate_type(const QValueList<UmlParameter> & params,
			  unsigned rank, QTextOStream & f)
{
  if (rank >= params.count())
    return FALSE;
  
  UmlClass::write(f, params[rank].type);
  return TRUE;
}

static bool generate_var(const QValueList<UmlParameter> & params, 
			 unsigned rank, QTextOStream & f)
{
  if (rank >= params.count())
    return FALSE;
  
  f << '$' << params[rank].name;
  return TRUE;
}

static bool generate_init(const QValueList<UmlParameter> & params, 
			  unsigned rank, QTextOStream & f)
{
  if (rank >= params.count())
    return FALSE;
  
  if (! params[rank].default_value.isEmpty())
    f << " = " << params[rank].default_value;
  
  return TRUE;
}

static void param_error(const QCString & parent, const QCString & name, unsigned rank)
{
  write_trace_header();
  UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>while compiling <i>")
		+ parent + "::" + name + "</i> parameter rank " + QCString().setNum(rank)
		+ " does not exist</font></b><br>");
  incr_error();
}

QCString UmlOperation::compute_name() {
  QCString get_set_spec = phpNameSpec();
  
  if (! get_set_spec.isEmpty()) {
    UmlClassMember * it;
    
    if ((it = getOf()) == 0)
      it = setOf();
    
    int index;
    QCString s = (it->kind() == aRelation)
      ? ((UmlRelation *) it)->roleName()
      : it->name();
    
    if ((index = get_set_spec.find("${name}")) != -1)
      get_set_spec.replace(index, 7, s);
    else if ((index = get_set_spec.find("${Name}")) != -1)
      get_set_spec.replace(index, 7, capitalize(s));
    else if ((index = s.find("${NAME}")) != -1)
      get_set_spec.replace(index, 7, s.upper());
    else if ((index = s.find("${nAME}")) != -1)
      get_set_spec.replace(index, 7, s.lower());
    
    return get_set_spec;
  }
  else
    return name();
}

// p0 is the beginning of the operation's def
// p point to ${body}
// indent is the one of the operation
const char * UmlOperation::generate_body(QTextOStream & f,
					 QCString indent,
					 const char * p)
{
  const char * body = 0;
  QCString modeler_body;
  bool no_indent;
  char s_id[9];
  
  if (preserve() && !isBodyGenerationForced()) {
    unsigned id = get_id();
    
    sprintf(s_id, "%08X", id);
    body = bodies.find((long) id);
  }
  
  if (body == 0) {
    no_indent = !phpContextualBodyIndent();
    modeler_body = phpBody();	// to not free the string
    body = modeler_body;
  }
  else
    // body from file, respect its indent
    no_indent = TRUE;
  
  // get keyword indent
  QCString bindent = indent;
  
  while (*p != '$')
    bindent += *p++;

  if (preserve() && !isBodyGenerationForced())
    f << bindent << BodyPrefix << s_id << '\n';

  if ((body != 0) && (*body != 0)) {
    // output body
    if (bindent.isEmpty() || no_indent) {
      f << body;
      body += strlen(body);
    }
    else {
      f << bindent;
      
      while (*body) {
	f << *body;
	if (*body++ == '\n') {
	  if (*body == 0)
	    break;
	  f << bindent;
	}
      }
    }
    
    if (body[-1] != '\n')
      f << '\n';
  }
  
  if (preserve() && !isBodyGenerationForced())
    f << bindent << BodyPostfix << s_id << '\n';
  
  f << indent;	// for the }
  
  return p + 7;
}

void UmlOperation::generate(QTextOStream & f, const QCString & cl_stereotype,
			    QCString indent, int &) {
  if (!phpDecl().isEmpty()) {
    const char * p = phpDecl();
    const char * pp = 0;
    const char * afterparam = 0;
    const QValueList<UmlParameter> & params = this->params();
    unsigned rank;
    const char * body_indent = strstr(p, "${body}");
    
    if (body_indent != 0) {
      while ((body_indent != p) &&
	     ((body_indent[-1] == ' ') || (body_indent[-1] == '\t')))
	body_indent -= 1;
    }
    
    // manage old style indent
    while ((*p == ' ') || (*p == '\t'))
      indent += *p++;
  
    f << indent;
    
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;
	if (*p == 0)
	  break;
	f << indent;
      }

      if (*p == '\n') {
	f << *p++;
	if (p == body_indent)
	  p = generate_body(f, indent, p);
	else if (*p)
	  f << indent;
      }
      else if (*p == '{') {
	if (isAbstract() || (cl_stereotype == "interface")) {
	  f << ";";
	  break;
	}
	else {
	  f << '{';
	  p += 1;
	}
      }
      else if (*p == '@')
	manage_alias(p, f);
      else if (*p != '$') {
	if (p == body_indent)
	  p = generate_body(f, indent, p);
	else
	  f << *p++;
      }
      else if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp, PhpSettings::isGenerateJavadocStyleComment());
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${visibility}", 13)) {
	p += 13;
	generate_visibility(f);
      }
      else if (!strncmp(p, "${final}", 8)) {
	p += 8;
	if (isPhpFinal())
	  f << "final ";
      }
      else if (!strncmp(p, "${static}", 9)) {
	p += 9;
	if (isClassMember())
	  f << "static ";
      }
      else if (!strncmp(p, "${abstract}", 11)) {
	p += 11;
	if (isAbstract() && (cl_stereotype != "interface"))
	  f << "abstract ";
      }
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	f << compute_name();
      }
      else if (!strncmp(p, "${(}", 4)) {
	p += 4;
	f << '(';
      }
      else if (!strncmp(p, "${)}", 4)) {
	p += 4;
	f << ')';
	afterparam = p;
      }
      else if (sscanf(p, "${t%u}", &rank) == 1) {
	if (!generate_type(params, rank, f))
	  param_error(parent()->name(), name(), rank);
	p = strchr(p, '}') + 1;
      }
      else if (sscanf(p, "${p%u}", &rank) == 1) {
	if (!generate_var(params, rank, f))
	  param_error(parent()->name(), name(), rank);
	p = strchr(p, '}') + 1;
      }
      else if (sscanf(p, "${v%u}", &rank) == 1) {
	if (!generate_init(params, rank, f))
	  param_error(parent()->name(), name(), rank);
	p = strchr(p, '}') + 1;
      }
      else if (!strncmp(p, "${staticnl}", 11)) {
	p += 11;
	if (isClassMember()) {
	  f << '\n';
	  if (*p)
	    f << indent;
	}
	else
	  f << ' ';
      }
      else if (!strncmp(p, "${body}", 7) &&
	       (pp == 0))	// not in comment
	p = generate_body(f, indent, p);
      else if (!strncmp(p, "${type}", 7)) {
	p += 7;
	UmlClass::write(f, returnType());
      }
      else
	f << *p++;
    }
    
    f << '\n';
  }
}

void UmlOperation::generate_require_onces(QTextOStream & f, QCString & made) {
  QCString s = phpDecl();
  
  if (!s.isEmpty()) {
    UmlArtifact * art = ((UmlClass *) parent())->assocArtifact();
			   
    returnType().generate_require_onces(f, made, art);
    
    int index1 = s.find("${(}");
    
    if (index1 == -1)
      return;
    
    index1 += 4;
    
    int index2 = s.find("${)}", index1);
    
    if(index2 == -1)
      return;
       
    s = s.mid((unsigned) index1, (unsigned) (index2 - index1));
    
    const QValueList<UmlParameter> & params = this->params();
    QValueListConstIterator<UmlParameter> it;
    unsigned rank;
    char ti[16];
    
    strcpy(ti, "${p");
    
    for (it = params.begin(), rank = 0;
	 it != params.end();
	 ++it, rank += 1) {
      sprintf(ti + 3, "%u}", rank);
      if (s.find(ti) != -1)
	(*it).type.generate_require_onces(f, made, art);
    }    
  }
}

//

static char * read_file(const char * filename)
{
  QFile fp(filename);
  
  if (fp.open(IO_ReadOnly)) {
    QFileInfo fi(fp);
    int size = fi.size();
    char * s = new char[size + 1];
    
    if (fp.readBlock(s, size) == -1) {
      delete [] s;
      return 0;
    }
    else {
      s[size] = 0;
      
      return s;
    }
  }
  else
    return 0;
}

static void read_bodies(const char * path, QIntDict<char> & bodies)
{
  char * s = read_file(path);
  
  if (s != 0) {
    char * p1 = s;
    char * p2;
    
    while ((p2 = strstr(p1, BodyPrefix)) != 0) {
      p2 += BodyPrefixLength;
      
      char * body;
      long id = strtol(p2, &body, 16);
      
      if (body != (p2 + 8)) {
	UmlCom::trace(QCString("<font color =\"red\"> Error in ") + path +
		      " : invalid preserve body identifier</font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("read_bodies 1");
      }
      
      if (bodies.find(id) != 0) {
	UmlCom::trace(QCString("<font  color =\"red\"> Error in ") + path + 
	  " : preserve body identifier used twice</font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("read_bodies 2");
      }
      
      if (*body == '\r')
	body += 1;
      if (*body == '\n')
	body += 1;
      else {
	UmlCom::trace(QCString("<font  color =\"red\"> Error in ") + path + 
		      " : invalid preserve body block, end of line expected</font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("read_bodies 3");
      }
      
      if (((p1 = strstr(body, BodyPostfix)) == 0) ||
	  (strncmp(p1 + BodyPostfixLength, p2, 8) != 0)) {
	UmlCom::trace(QCString("<font  color =\"red\"> Error in ") + path + 
		      " : invalid preserve body block, wrong balanced</font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("read_bodies 4");
      }

      p2 = p1;
      while ((p2 != body) && (p2[-1] != '\n'))
	p2 -= 1;
      *p2 = 0;
      
      int len = p2 - body + 1;
      char * b = new char[len];
      
      memcpy(b, body, len);
      bodies.insert(id, b);
      
      p1 += BodyPostfixLength + 8;
    }
    
    delete [] s;
  }
}

void UmlOperation::read_bodies(const char * path)
{
  bodies.setAutoDelete(TRUE);
  bodies.clear();
  ::read_bodies(path, bodies);
}
