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

#include <qtextstream.h> 

#include "UmlRelation.h"
#include "UmlClass.h"
#include "UmlSettings.h"
#include "PhpSettings.h"
#include "util.h"
#include "UmlCom.h"

void UmlRelation::generate_extends(const char *& sep, QTextOStream & f,
				   const QCString & cl_stereotype) {
  switch (relationKind()) {
  default:
    return;
  case aGeneralisation:
  case aRealization:
    if (phpDecl().isEmpty())
      return;
    
    UmlClass * role_type = roleType();
    const QCString & other_stereotype = role_type->php_stereotype();
    
    if ((cl_stereotype == "interface") || (cl_stereotype == "@interface")) {
      if ((other_stereotype != "interface") && (other_stereotype != "@interface")) {
	write_trace_header();
	UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>cannot extends a <i>")
		      + other_stereotype + "</i></b></font><br>");
	incr_warning();
	return;
      }
    }
    else if ((other_stereotype != "interface") && (other_stereotype != "@interface")) {
      if (cl_stereotype == "union") {
	write_trace_header();
	UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>")
		      + cl_stereotype + "</i> cannot extends</b></font><br>");
	incr_warning();
	return;
      }
      else if ((other_stereotype == "union") ||
	       (other_stereotype == "enum")) {
	write_trace_header();
	UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>cannot extends an <i>")
		      + other_stereotype + "</i></b></font><br>");
	incr_warning();
	return;
      }
      else if (*sep == ',') {
	write_trace_header();
	UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>extend several classes</b></font><br>");
	incr_warning();
	return;
      }
    }
    else
      return;
    
    // here the code is legal
    f << sep;
    sep = ", ";
    
    const char * p = phpDecl();
    
    while (*p) {
      if (!strncmp(p, "${type}", 7)) {
	role_type->write(f);
	p += 7;
      }
      else if (*p == '@')
	manage_alias(p, f);
      else
	f << *p++;
    }
  }
}

void UmlRelation::generate_implements(const char *& sep, QTextOStream & f,
				      const QCString & cl_stereotype) {
  switch (relationKind()) {
  default:
    return;
  case aGeneralisation:
  case aRealization:
    if (phpDecl().isEmpty()) 
      return;
    
    UmlClass * role_type = roleType();
    const QCString & other_stereotype = role_type->php_stereotype();
    
    if (other_stereotype == "interface") {
      if (cl_stereotype == "union") {
	write_trace_header();
	UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>")
		      + cl_stereotype + "</i> cannot inherits</b></font><br>");
	incr_warning();
      }
      else {
	f << sep;
	sep = ", ";
	
	const char * p = phpDecl();
	
	while (*p) {
	  if (!strncmp(p, "${type}", 7)) {
	    role_type->write(f);
	    p += 7;
	  }
	  else if (*p == '@')
	    manage_alias(p, f);
	  else
	    f << *p++;
	}
      }
    }
  }
}

void UmlRelation::generate(QTextOStream & f, const QCString &,
			   QCString indent, int &) {
  switch (relationKind()) {
  case aDependency:
  case aGeneralisation:
  case aRealization:
    break;
  default:
    if (!phpDecl().isEmpty()) {
      const char * p = phpDecl();
      const char * pp = 0;
      QCString s;
      
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
	  if (*p)
	    f << indent;
	}
	else if (*p == '@')
	  manage_alias(p, f);
	else if (*p != '$')
	  f << *p++;
	else if (!strncmp(p, "${comment}", 10))
	  manage_comment(p, pp, PhpSettings::isGenerateJavadocStyleComment());
	else if (!strncmp(p, "${description}", 14))
	  manage_description(p, pp);
	else if (!strncmp(p, "${visibility}", 13)) {
	  p += 13;
	  generate_visibility(f);
	}
	else if (!strncmp(p, "${static}", 9)) {
	  p += 9;
	  if (isClassMember())
	    f << "static ";
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  roleType()->write(f);
	}
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  if (!isReadOnly())
	    f << "$";
	  f << roleName();
	}
	else if (!strncmp(p, "${inverse_name}", 15)) {
	  p += 15;
	  switch (relationKind()) {
	  case anAssociation:
	  case anAggregation:
	  case anAggregationByValue:
	    {
	      UmlRelation * inverse = side(side(TRUE) != this);
	      
	      if (!inverse->isReadOnly())
		f << "$";
	      f << inverse->roleName();
	    }
	  default:
	    break;
	  }
	}
	else if (!strncmp(p, "${var}", 6)) {
	  p += 6;
	  if (!isReadOnly() &&
	      !isClassMember() &&
	      (visibility() == PackageVisibility))
	    f << "var ";
	}
	else if (!strncmp(p, "${value}", 8)) {
	  if (!defaultValue().isEmpty()) {
	    if (need_equal(p, defaultValue()))
	      f << " = ";
	    f << defaultValue();
	  }
	  p += 8;
	}
	else if (!strncmp(p, "${const}", 8)) {
	  p += 8;
	  if (isReadOnly())
	    f << "const ";
	}
	else
	  f << *p++;
      }
    
      f << '\n';
    }
  }
}

void UmlRelation::generate_require_onces(QTextOStream & f, QCString & made) {
  if (!phpDecl().isEmpty())
    roleType()->generate_require_onces(f, made, ((UmlClass *) parent())->assocArtifact());
}
