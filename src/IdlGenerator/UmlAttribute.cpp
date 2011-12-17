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

#include "UmlAttribute.h"
#include "UmlSettings.h"
#include "IdlSettings.h"
#include "UmlClass.h"
#include "UmlCom.h"
#include "util.h"

void UmlAttribute::generate_decl(QTextOStream & f, const QCString & cl_stereotype,
				 QCString indent, bool islast) {
  if (!idlDecl().isEmpty()) {
    if (cl_stereotype == "typedef") {
      write_trace_header();
      UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>a <i>typedef</i> cannot have attribute</b></font><br>");
      incr_warning();
      return;
    }
    const char * p = idlDecl();
    const char * pp = 0;
    bool in_enum = (cl_stereotype == "enum");
    
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
	if (*p)
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
      else if (!strncmp(p, "${name}", 7)) {
	p += (in_enum && islast && (p[7] == ','))
	  ? 8 : 7;
	f << name();
      }
      else if (!strncmp(p, "${stereotype}", 13)) {
	p += 13;
	f << IdlSettings::relationAttributeStereotype(stereotype());
      }
      else if (!strncmp(p, "${multiplicity}", 15)) {
	p += 15;
	f << multiplicity();
      }
      else if (!strncmp(p, "${value}", 8)) {
	if (!defaultValue().isEmpty() && isReadOnly() && isClassMember()) {
	  if (need_equal(p, defaultValue()))
	    f << " = ";
	  f << defaultValue();
	}
	p += 8;
      }
      else if (in_enum)
	// strange
	f << *p++;
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
	  f << idl_case;
      }
      else if (!strncmp(p, "${readonly}", 11)) {
	p += 11;
	if (isReadOnly())
	  f << "readonly ";
      }
      else if (!strncmp(p, "${type}", 7)) {
	p += 7;
	UmlClass::write(f, type());
      }
      else
	// strange
	f << *p++;
    }
      
    f << '\n';
  }
}
