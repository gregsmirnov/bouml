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
#include "PythonSettings.h"
#include "UmlCom.h"
#include "UmlClass.h"
#include "UmlRelation.h"
#include "util.h"

// to manage preserved bodies, the key is the id under bouml
QIntDict<char> UmlOperation::bodies(127);
const char * BodyPrefix = "## Bouml preserved body begin ";
const char * BodyPostfix = "## Bouml preserved body end ";
const int BodyPrefixLength = 30;
const int BodyPostfixLength = 28;

void UmlOperation::generate_imports(QTextOStream & f, QCString & made) {
  QCString s = pythonDecl();
  
  if (!s.isEmpty()) {
    UmlArtifact * art = ((UmlClass *) parent())->assocArtifact();
			   
    returnType().generate_import(f, art, FALSE, made);
    
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
	(*it).type.generate_import(f, art, FALSE, made);
    }    
  }
}

static bool generate_type(const QValueList<UmlParameter> & params,
			  unsigned rank, QTextOStream & f, bool in_params)
{
  if (rank >= params.count())
    return FALSE;
  
  const UmlTypeSpec & t = params[rank].type;
  
  if (in_params && ((t.type != 0) || !t.explicit_type.isEmpty()))
    f << ": ";
  
  UmlClass::write(f, t);
  return TRUE;
}

static bool generate_var(const QValueList<UmlParameter> & params, 
			 unsigned rank, QTextOStream & f)
{
  if (rank >= params.count())
    return FALSE;
  
  f << params[rank].name;
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
  QCString get_set_spec = pythonNameSpec();
  
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

// p point to ${body}
const char * UmlOperation::generate_body(QTextOStream & f,
					 QCString indent,
					 BooL & indent_needed,
					 const char * p)
{
  const char * body = 0;
  QCString modeler_body;
  QCString body_indent;
  char s_id[9];
  
  if (preserve() && !isBodyGenerationForced()) {
    unsigned id = get_id();
    
    sprintf(s_id, "%08X", id);
    body = bodies.find((long) id);
  }
  
  if (body == 0) {
    modeler_body = pythonBody();	// to not free the string
    body = modeler_body;
    if (pythonContextualBodyIndent())
      body_indent = indent;
  }
  
  if (name() == "__init__") {
    const QValueList<UmlParameter> & params = this->params();
    
    if (params.count() != 0)
      ((UmlClass *) parent())->generate_instance_att_rel(f, indent, indent_needed,
							 params[0].name + ".");
    else {
      QCString err = "&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b><i>" +
	parent()->name() + ".__init__()</i> doesn't have parameter, instance variables not generated </b></font><br>";
      
      write_trace_header();
      UmlCom::trace(err);
      incr_warning();
    }      
  }
  
  if (preserve() && !isBodyGenerationForced()) {
    if (indent_needed)
      f << indent;
    f << BodyPrefix << s_id << '\n' << body_indent;
  }
  else if (indent_needed)
    f << body_indent;
  
  if ((body != 0) && (*body != 0)) {
    // output body
    while (*body) {
      f << *body;
      if (*body++ == '\n') {
	if (*body == 0)
	  break;
	f << body_indent;
      }
    }
    
    if (body[-1] != '\n')
      f << '\n';
  }
  else
    f << "pass\n";
  
  if (preserve() && !isBodyGenerationForced())
    f << indent << BodyPostfix << s_id << '\n';

  indent_needed = TRUE;
  
  return p + 7;
}


static void manage_decorators(QTextOStream & f, const QCString & decorators,
			      QString indent, BooL & indent_needed)
{
  if (! decorators.isEmpty()) {
    int index = 0;
    int index2;
    
    while ((index2 = decorators.find("\n", index)) != -1){
      if (indent_needed)
	f << indent;
      else
	indent_needed = TRUE;
      f << decorators.mid(index, index2 + 1 - index);
      index = index2 + 1;
    }
    
    if (index != (int) decorators.length()) {
      if (indent_needed) {
	f << indent;
	indent_needed = FALSE;
      }
      f << decorators.mid(index);
    }
  }
}

void UmlOperation::generate(QTextOStream & f, const QCString &,
			    QCString indent, BooL & indent_needed,
			    int &, const QCString &) {
  const char * p = pythonDecl();
  
  if ((p == 0) || (*p == 0))
    return;
  
  const char * pp = 0;
  QCString saved_indent = indent;
  QCString indent_step = PythonSettings::indentStep();
  const char * afterparam = 0;
  const QValueList<UmlParameter> & params = this->params();
  unsigned rank;
  bool isinline = TRUE;
  bool in_params = FALSE;
    
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      if (*p == 0)
	break;
      pp = 0;
      indent = saved_indent;
    }

    if (*p == '@')
      manage_alias(p, f, indent, indent_needed);
    else if (*p != '$') {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      switch (*p) {
      case ':':
	if (pp == 0) {
	  indent += indent_step;
	  saved_indent = indent;
	  indent_step = "";
	}
	break;
      case '\n':
	indent_needed = TRUE;
	break;
      }
      f << *p++;
    }
    else if (!strncmp(p, "${comment}", 10))
      manage_comment(p, pp);
    else if (!strncmp(p, "${description}", 14))
      manage_description(p, pp);
    else if (!strncmp(p, "${docstring}", 12))
      manage_docstring(p, pp, indent_needed, indent, saved_indent);
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (isClassMember()) {
	if (indent_needed)
	  f << indent;
	else
	  indent_needed = TRUE;
	f << "@staticmethod\n";
      }
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (isAbstract()) {
	if (indent_needed)
	  f << indent;
	else
	  indent_needed = TRUE;
	f << "@abstractmethod\n";
      }
    }
    else if (!strncmp(p, "${@}", 4)) {
      p += 4;
      manage_decorators(f, pythonDecorators(), indent, indent_needed);
    }
    else if (!strncmp(p, "${name}", 7)) {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      p += 7;
      f << compute_name();
    }
    else if (!strncmp(p, "${class}", 8)) {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      p += 8;
      f << parent()->name();
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      f << '(';
      in_params = TRUE;
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      f << ')';
      afterparam = p;
      in_params = FALSE;
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      if (!generate_type(params, rank, f, in_params))
	param_error(parent()->name(), name(), rank);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      if (!generate_var(params, rank, f))
	param_error(parent()->name(), name(), rank);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      if (!generate_init(params, rank, f))
	param_error(parent()->name(), name(), rank);
      p = strchr(p, '}') + 1;
    }
    else if (!strncmp(p, "${body}", 7) &&
	     (pp == 0))	{// not in comment
      isinline = FALSE;
      p = generate_body(f, indent, indent_needed, p);
    }
    else if (!strncmp(p, "${association}", 14)) {
      p += 14;
      
      UmlClassMember * m;
      
      if ((((m = getOf()) != 0) || ((m = setOf()) != 0)) &&
	  (m->kind() == aRelation)) {
	if (indent_needed) {
	  indent_needed = FALSE;
	  f << indent;
	}
	UmlClass::write(f, ((UmlRelation *) m)->association());
      }
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      
      const UmlTypeSpec & t = returnType();
  
      if ((t.type != 0) || !t.explicit_type.isEmpty()) {
	f << " -> ";
  
	UmlClass::write(f, t);
      }
    }
    else {
      // strange
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      f << *p++;
    }
  }
  
  if (isinline) {
    if (indent_needed)
      f << indent << '\n';
    else
      f << '\n';
    indent_needed = TRUE;
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
