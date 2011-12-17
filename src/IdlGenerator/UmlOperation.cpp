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
#include <qtextstream.h>

#include "UmlOperation.h"
#include "UmlSettings.h"
#include "IdlSettings.h"
#include "UmlCom.h"
#include "UmlClass.h"
#include "UmlRelation.h"
#include "util.h"

static bool generate_dir(const QValueList<UmlParameter> & params,
			 unsigned rank, QTextOStream & f)
{
  if (rank >= params.count())
    return FALSE;
  
  switch (params[rank].dir) {
  case InputOutputDirection:
    f << "inout";
    break;
  case InputDirection:
    f << "in";
    break;
  default:
    f << "out";
  }
  return TRUE;
}

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
  
  f << params[rank].name;
  return TRUE;
}

static void param_error(const QCString & parent, const QCString & name,
			unsigned rank, const char * where)
{
  write_trace_header();
  UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>while compiling <i>")
		+ parent + "::" + name + "</i> " + where
		+ ", parameter rank " + QCString().setNum(rank)
		+ " does not exist</font></b><br>");
  incr_error();
}

QCString UmlOperation::compute_name() {
  QCString get_set_spec = idlNameSpec();
  
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

void UmlOperation::generate_decl(QTextOStream & f,
				 const QCString & cl_stereotype,
				 QCString indent, bool) {
  if (!idlDecl().isEmpty()) {
    if ((cl_stereotype != "interface") &&
	(cl_stereotype != "valuetype")) {
      write_trace_header();
      UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>cannot have operation</b></font><br>");
      incr_warning();
      return;
    }
    
    const char * p = idlDecl();
    const char * pp = 0;
    const QValueList<UmlParameter> & params = this->params();
    const QValueList<UmlTypeSpec> & exceptions = this->exceptions();
    unsigned rank;
    
    while ((*p == ' ') || (*p == '\t'))
      indent += *p++;
  
    if (*p != '#')
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
	if (*p != '#')
	  f << indent;
      }
      
      if (*p == '\n') {
	f << *p++;
	if (*p && (*p != '#'))
	  f << indent;
      }
      else if (*p == '@')
	manage_alias(p, f);
      else if (*p != '$')
	f << *p++;
      else if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp);
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${oneway}", 9)) {
	p += 9;
	if (isIdlOneway())
	  f << "oneway ";
      }
      else if (!strncmp(p, "${type}", 7)) {
	p += 7;
	UmlClass::write(f, returnType());
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
      }
      else if (sscanf(p, "${d%u}", &rank) == 1) {
	if (!generate_dir(params, rank, f))
	  param_error(parent()->name(), name(), rank, "declaration");
	p = strchr(p, '}') + 1;
      }
      else if (sscanf(p, "${t%u}", &rank) == 1) {
	if (!generate_type(params, rank, f))
	  param_error(parent()->name(), name(), rank, "declaration");
	p = strchr(p, '}') + 1;
      }
      else if (sscanf(p, "${p%u}", &rank) == 1) {
	if (!generate_var(params, rank, f))
	  param_error(parent()->name(), name(), rank, "declaration");
	p = strchr(p, '}') + 1;
      }
      else if (!strncmp(p, "${stereotype}", 13)) {
	p += 13;
	f << IdlSettings::relationAttributeStereotype(stereotype());
      }
      else if (!strncmp(p, "${raisesnl}", 11)) {
	if (exceptions.count() != 0) {
	  f << '\n' << indent << "  ";
	}
	p += 11;
      }
      else if (!strncmp(p, "${raises}", 9)) {
	p += 9;
		
	if (! exceptions.isEmpty()) {
	  const char * sep;
	  QValueList<UmlTypeSpec>::ConstIterator it;
	  
	  for (it = exceptions.begin(), sep = " raises ("; 
	       it != exceptions.end();
	       ++it, sep = " ")
	    f << sep << IdlSettings::type((*it).toString());
	    
	  f << ')';
	}
      }
      else if (!strncmp(p, "${association}", 14)) {
	p += 14;
	
	UmlClassMember * m;
	
	if (((m = getOf()) != 0) && (m->kind() == aRelation))
	  f << IdlSettings::type(((UmlRelation *) m)->association().toString());
	else if (((m = setOf()) != 0) && (m->kind() == aRelation))
	  f << IdlSettings::type(((UmlRelation *) m)->association().toString());
      }
      else
	// strange
	f << *p++;
    }
    f << '\n';
  }
}

