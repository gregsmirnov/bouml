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
#include "PhpSettings.h"
#include "UmlClass.h"
#include "UmlCom.h"
#include "util.h"

void UmlAttribute::generate(QTextOStream & f, const QCString & st,
			    QCString indent, int & enum_item_rank) {
  if (!phpDecl().isEmpty()) {
    const char * p = phpDecl();
    const char * pp = 0;
    
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
	UmlClass::write(f, type());
      }
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	if ((st != "enum") && !isReadOnly())
	  f << "$";
	f << name();
      }
      else if (!strncmp(p, "${var}", 6)) {
	p += 6;
	if ((st != "enum") &&
	    !isReadOnly() &&
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
	else if (st == "enum")
	  f << " = " << enum_item_rank;
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
  
  enum_item_rank += 1;
}

void UmlAttribute::generate_require_onces(QTextOStream & f, QCString & made) {
  if (!phpDecl().isEmpty())
    type().generate_require_onces(f, made, ((UmlClass *) parent())->assocArtifact());
}
