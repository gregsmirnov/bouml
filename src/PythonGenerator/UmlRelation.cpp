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
#include "UmlArtifact.h"
#include "UmlPackage.h"
#include "UmlSettings.h"
#include "PythonSettings.h"
#include "util.h"
#include "UmlCom.h"


void UmlRelation::generate_imports(QTextOStream & f, QCString & made) {
  if (!pythonDecl().isEmpty()) {
    bool from;
    
    if (relationKind() == aDependency) {
      QCString s = stereotype();
      
      if (s == "import")
	from = FALSE;
      else if (s == "from")
	from = TRUE;
      else
	return;
    }
    else
      from = FALSE;

    roleType()->generate_import(f, ((UmlClass *) parent())->assocArtifact(), 
				from, made);
  }
}

void UmlRelation::generate_inherit(const char *& sep, QTextOStream & f) {
  switch (relationKind()) {
  default:
    return;
  case aGeneralisation:
  case aRealization:
    if (pythonDecl().isEmpty())
      return;
    
    f << sep;
    sep = ", ";
    
    const char * p = pythonDecl();
    
    while (*p) {
      if (!strncmp(p, "${type}", 7)) {
	roleType()->write(f);
	p += 7;
      }
      else if (*p == '@') {
	QCString indent;
	BooL indent_needed = FALSE;
	
	manage_alias(p, f, indent, indent_needed);
      }
      else
	f << *p++;
    }
  }
}

void UmlRelation::generate(QTextOStream & f, const QCString &,
			   QCString indent, BooL & indent_needed,
			   int &, const QCString & self) {
  switch (relationKind()) {
  case aDependency:
  case aGeneralisation:
  case aRealization:
    return;
  default:
    break;
  }
  
  if (self.isEmpty() != isClassMember())
    return;
  
  const char * p = pythonDecl();
  
  if ((p == 0) || (*p == 0))
    return;
      
  const char * pp = 0;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      if (*p == 0)
	break;
      pp = 0;
    }
    
    if (*p != '$') {
      if (*p == '@')
	manage_alias(p, f, indent, indent_needed);
      else {
	if (indent_needed)
	  f << indent;
	indent_needed = (*p == '\n');
	
	f << *p++;
      }
    }
    else if (!strncmp(p, "${comment}", 10))
      manage_comment(p, pp);
    else if (!strncmp(p, "${description}", 14))
      manage_description(p, pp);
    else if (!strncmp(p, "${name}", 7)) {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      p += 7;
      f << roleName();
    }
    else if (!strncmp(p, "${inverse_name}", 15)) {
      switch (relationKind()) {
      case anAssociation:
      case anAggregation:
      case anAggregationByValue:
	if (indent_needed) {
	  indent_needed = FALSE;
	  f << indent;
	}
	f << side(side(TRUE) != this)->roleName();
      default:
	break;
      }
      p += 15;
    }
    else if (!strncmp(p, "${value}", 8)) {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      
      const QCString & v = defaultValue();
      
      if (!v.isEmpty()) {
	if (need_equal(p, v))
	  f << " = ";
	f << v;
      }
      else if (need_equal(p, "None"))
	f << " = None";
      else
	f << "None";
      p += 8;
    }
    else if (!strncmp(p, "${self}", 7)) {
      p += 7;
      if (! self.isEmpty()) {
	if (indent_needed) {
	  indent_needed = FALSE;
	  f << indent;
	}
	f << self;
      }
    }
    else if (!strncmp(p, "${stereotype}", 13)) {
      p += 13;
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      f << PythonSettings::relationAttributeStereotype(stereotype());
    }
    else if (!strncmp(p, "${type}", 7)) {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      p += 7;
      roleType()->write(f);
    }
    else if (!strncmp(p, "${association}", 14)) {
      if (indent_needed) {
	indent_needed = FALSE;
	f << indent;
      }
      p += 14;
      UmlClass::write(f, association());
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
  
  if (! indent_needed) {
    f << '\n';
    indent_needed = TRUE;
  }
}

