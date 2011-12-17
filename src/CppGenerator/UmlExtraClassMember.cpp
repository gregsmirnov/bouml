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

#include "UmlExtraClassMember.h"
#include "UmlClass.h"

void UmlExtraClassMember::compute_dependency(QList<CppRefType> &,
					     const QCString &,
					     bool) {
}

void UmlExtraClassMember::generate_decl(aVisibility &,
					QTextOStream & f_h,
					const QCString &,
					QCString indent,
					BooL & first, bool) {
  first = FALSE;
  
  const char * p = cppDecl();
  
  if (*p != '#')
    f_h << indent;
  
  f_h << p;
}

void UmlExtraClassMember::generate_def(QTextOStream & f, QCString indent,
				       bool h, QCString templates,
				       QCString, QCString, QCString) {
  if (!cppDef().isEmpty() &&
      ((isCppInline() || !templates.isEmpty()) == h)) {
    const char * p = cppDef();
    
    if (*p != '#')
      f << indent;
    
    f << p;
  }
}
