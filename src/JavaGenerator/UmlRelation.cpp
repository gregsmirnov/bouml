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
#include "JavaSettings.h"
#include "util.h"
#include "UmlCom.h"

void UmlRelation::generate_extends(const char *& sep, QTextOStream & f,
				   const QValueList<UmlActualParameter> & actuals,
				   const QCString & cl_stereotype) {
  switch (relationKind()) {
  default:
    return;
  case aGeneralisation:
  case aRealization:
    if (javaDecl().isEmpty())
      return;
    
    UmlClass * role_type = roleType();
    const QCString & other_stereotype = role_type->java_stereotype();
    
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
      if ((cl_stereotype == "union") || (cl_stereotype == "enum_pattern")) {
	write_trace_header();
	UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>")
		      + cl_stereotype + "</i> cannot extends</b></font><br>");
	incr_warning();
	return;
      }
      else if ((other_stereotype == "union") ||
	       (other_stereotype == "enum") ||
	       (other_stereotype == "enum_pattern")) {
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
    
    const char * p = javaDecl();
    
    while (*p) {
      if (!strncmp(p, "${type}", 7)) {
	role_type->write(f);
	p += 7;
    
	if (!actuals.isEmpty()) {
	  QValueList<UmlActualParameter>::ConstIterator ita;
	  bool used = FALSE;
	  
	  for (ita = actuals.begin(); ita != actuals.end(); ++ita) {
	    if ((*ita).superClass() == role_type) {
	      used = TRUE;
	      (*ita).generate(f);
	    }
	  }
	  
	  if (used) {
	    f << ">";
	  }
	}
      }
      else if (*p == '@')
	manage_alias(p, f);
      else
	f << *p++;
    }
  }
}

void UmlRelation::generate_implements(const char *& sep, QTextOStream & f,
				      const QValueList<UmlActualParameter> & actuals,
				      const QCString & cl_stereotype) {
  switch (relationKind()) {
  default:
    return;
  case aGeneralisation:
  case aRealization:
    if (javaDecl().isEmpty()) 
      return;
    
    UmlClass * role_type = roleType();
    const QCString & other_stereotype = role_type->java_stereotype();
    
    if ((other_stereotype == "interface") || (other_stereotype == "@interface")) {
      if ((cl_stereotype == "union") || (cl_stereotype == "enum_pattern")) {
	write_trace_header();
	UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>")
		      + cl_stereotype + "</i> cannot inherits</b></font><br>");
	incr_warning();
      }
      else {
	f << sep;
	sep = ", ";
	
	const char * p = javaDecl();
	
	while (*p) {
	  if (!strncmp(p, "${type}", 7)) {
	    role_type->write(f);
	    p += 7;
	    
	    if (!actuals.isEmpty()) {
	      QValueList<UmlActualParameter>::ConstIterator ita;
	      bool used = FALSE;
	      
	      for (ita = actuals.begin(); ita != actuals.end(); ++ita) {
		if ((*ita).superClass() == role_type) {
		  used = TRUE;
		  (*ita).generate(f);
		}
	      }
	      
	      if (used) {
		f << ">";
	      }
	    }
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

void UmlRelation::generate(QTextOStream & f, const QCString & cl_stereotype,
			   QCString indent) {
  switch (relationKind()) {
  case aDependency:
  case aGeneralisation:
  case aRealization:
    break;
  default:
    if (!javaDecl().isEmpty()) {
      if (cl_stereotype == "enum_pattern") {
	write_trace_header();
	UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>enum_pattern</i> cannot have relation</b></font><br>");
	incr_warning();
	return;
      }
      
      const char * p = javaDecl();
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
	  manage_comment(p, pp, JavaSettings::isGenerateJavadocStyleComment());
	else if (!strncmp(p, "${description}", 14))
	  manage_description(p, pp);
	else if (!strncmp(p, "${visibility}", 13)) {
	  p += 13;
	  generate_visibility(f, cl_stereotype);
	}
	else if (!strncmp(p, "${static}", 9)) {
	  p += 9;
	  if (isClassMember())
	    f << "static ";
	}
	else if (!strncmp(p, "${final}", 8)) {
	  p += 8;
	  if (isReadOnly())
	    f << "final ";
	}
	else if (!strncmp(p, "${transient}", 12)) {
	  p += 12;
	  if (isJavaTransient())
	    f << "transient ";
	}
	else if (!strncmp(p, "${volatile}", 11)) {
	  p += 11;
	  if (isVolatile())
	    f << "volatile ";
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  roleType()->write(f);
	}
	else if (!strncmp(p, "${stereotype}", 13)) {
	  p += 13;
	  f << JavaSettings::relationAttributeStereotype(stereotype());
	}
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  f << roleName();
	}
	else if (!strncmp(p, "${inverse_name}", 15)) {
	  p += 15;
	  switch (relationKind()) {
	  case anAssociation:
	  case anAggregation:
	  case anAggregationByValue:
	    f << side(side(TRUE) != this)->roleName();
	  default:
	    break;
	  }
	}
	else if (!strncmp(p, "${multiplicity}", 15)) {
	  p += 15;
	  
	  QCString m = multiplicity();
	  
	  if (*m != '[')
	    f << "[]";
	  else {
	    for (unsigned index = 0; index != m.length(); index += 1) {
	      switch (m.at(index)) {
	      case '[':
		f << '[';
		break;
	      case ']':
		f << ']';
	      default:
		break;
	      }
	    }
	  }
	}
	else if (!strncmp(p, "${value}", 8)) {
	  if (!defaultValue().isEmpty()) {
	    if (need_equal(p, defaultValue()))
	      f << " = ";
	    f << defaultValue();
	  }
	  p += 8;
	}
	else if (!strncmp(p, "${association}", 14)) {
	  p += 14;
	  UmlClass::write(f, association());
	}
	else if (!strncmp(p, "${@}", 4)) {
	  p += 4;
	  if (pp != 0)
	    f << "${@}";
	  else if (! javaAnnotations().isEmpty()) {
	    pp =p;
	    p = javaAnnotations();
	  }
	}
	else
	  // strange
	  f << *p++;
      }
    
      f << '\n';
    }
  }
}

void UmlRelation::generate_enum_pattern_item(QTextOStream &, int &,
					     const QCString &, QCString) {
  write_trace_header();
  UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>enum_pattern</i> cannot have relation</b></font><br>");
  incr_warning();
}

void UmlRelation::generate_enum_pattern_case(QTextOStream &, QCString) {
  // error already signaled
}

void UmlRelation::generate_enum_member(QTextOStream & f, QCString indent) {
  generate(f, "enum", indent);
}

void UmlRelation::generate_import(QTextOStream & f, const QCString & indent) {
  if ((relationKind() == aDependency) &&
      (stereotype() == "import"))
    roleType()->import(f, indent);
}
