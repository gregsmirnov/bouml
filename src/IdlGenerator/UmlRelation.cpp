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
#include "IdlSettings.h"
#include "UmlCom.h"
#include "util.h"

void UmlRelation::generate_inherit(const char *& sep, QTextOStream & f, 
				   const QCString & cl_stereotype,
				   BooL & already) {
  if ((relationKind() == aGeneralisation) || (relationKind() == aRealization)) {
    UmlClass * role_type = roleType();
    QCString other_stereotype = role_type->idl_stereotype();
    
    if (other_stereotype == "ignored")
      return;
    
    if ((cl_stereotype == "union") || (cl_stereotype == "enum")) {
      write_trace_header();
      UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>")
		    + cl_stereotype + "</i> cannot inherits</b></font><br>");
      incr_warning();
      return;
    }
    
    if ((other_stereotype == "union") ||
	(other_stereotype == "struct") ||
	(other_stereotype == "enum") ||
	(other_stereotype == "typedef")) {
      write_trace_header();
      UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>cannot inherits an <i>")
		    + other_stereotype + "</i></b></font><br>");
      incr_warning();
      return;
    }
    
    if (cl_stereotype == "valuetype") {
      if (other_stereotype == "valuetype") {
	if (isIdlTruncatableInheritance())
	  f << sep << "truncatable ";
	else
	  f << sep;
      }
      else {
	// other_stereotype == "interface"
	if (!already) {
	  already = TRUE;
	  f << " supports ";
	}
	else
	  f << sep;
      }
    }
    else
      f << sep;
    
    const char * p = idlDecl();
    
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
    
    sep = ", ";
  }
}

void UmlRelation::generate_decl(QTextOStream & f,
				const QCString & cl_stereotype,
				QCString indent, bool) {
  if ((relationKind() != aGeneralisation) && 
      (relationKind() != aRealization) && 
      (relationKind() != aDependency)) {
    if (cl_stereotype == "enum") {
      write_trace_header();
      UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>enum</i> cannot have relation</b></font><br>");
      incr_warning();
      return;
    }
    if (cl_stereotype == "typedef") {
      write_trace_header();
      UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>a <i>typedef</i> cannot have relation</b></font><br>");
      incr_warning();
      return;
    }
    if (!idlDecl().isEmpty()) {
      const char * p = idlDecl();
      const char * pp = 0;
      QCString s;
      
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
	else if (!strncmp(p, "${readonly}", 11)) {
	  p += 11;
	  if (isReadOnly())
	    f << "readonly ";
	}
	else if (!strncmp(p, "${attribut}", 11)) {
	  // old version
	  p += 11;
	  if (cl_stereotype == "interface")
	    f << "attribute ";
	}
	else if (!strncmp(p, "${attribute}", 12)) {
	  p += 12;
	  if (cl_stereotype == "interface")
	    f << "attribute ";
	}
	else if (!strncmp(p, "${visibility}", 13)) {
	  p += 13;
	  if (cl_stereotype == "valuetype") {
	    switch (visibility()) {
	    case PublicVisibility:
	    case PackageVisibility:
	      f << "public ";
	      break;
	    default:
	      f << "private ";
	    } 
	  }
	}
	else if (!strncmp(p, "${case}", 7)) {
	  p += 7;
	  
	  QCString idl_case = idlCase();
	  
	  if (idl_case.isEmpty()) {
	    write_trace_header();
	    UmlCom::trace(QCString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>unspecified <i>case</i> for <i>")
			  + name() + "</b></font><br>");
	    incr_error();
	  }
	  else
	    f << idlCase();
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  roleType()->write(f);
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
	else if (!strncmp(p, "${stereotype}", 13)) {
	  p += 13;
	  f << IdlSettings::relationAttributeStereotype(stereotype());
	}
	else if (!strncmp(p, "${multiplicity}", 15)) {
	  p += 15;
	  f << multiplicity();
	}
	else if (!strncmp(p, "${association}", 14)) {
	  p += 14;
	  f << IdlSettings::type(association().toString());
	}
	else
	  // strange
	  f << *p++;
      }
    
      f << '\n';
    }
  }
}

