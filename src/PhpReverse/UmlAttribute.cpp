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

#ifdef TRACE
#include <iostream>

using namespace std;
#endif

#include "UmlAttribute.h"
#include "UmlPackage.h"
#include "UmlOperation.h"
#include "Package.h"
#include "UmlClass.h"
#include "Class.h"
#include "PhpCatWindow.h"
#include "PhpSettings.h"
#include "Lex.h"
#ifdef REVERSE
#include "Statistic.h"
#endif

bool UmlAttribute::new_one(Class * container, QCString name,
			   aVisibility visibility, bool constp,
			   bool staticp, const QCString & value,
			   QCString comment, QCString description)
{
#ifdef TRACE
  cout << "ATTRIBUTE '" << name << "'\n";
#endif
  
#ifndef REVERSE
  if (visibility == PrivateVisibility)
    return TRUE;
#endif
  
  if (((const char *) name)[0] == '$')
    name = name.mid(1);
  
  UmlClass * cl = container->get_uml();
  UmlAttribute * at = UmlBaseAttribute::create(cl, name);
  
  if (at == 0) {
    PhpCatWindow::trace(QCString("<font face=helvetica><b>cannot add attribute <i>")
			 + name + "</i> in <i>" + cl->name() 
			 + "</i></b></font><br>");  
    return FALSE;
  }
#ifdef REVERSE
  Statistic::one_attribute_more();
#endif
  
  if (!comment.isEmpty()) {
    QCString s = (at->phpDecl().find("${description}") != -1)
      ? description : comment;
    UmlTypeSpec t;
    int index;
    
    if (! (t.explicit_type = value_of(s, "@var", index)).isEmpty()) {
      at->set_Type(t);
      s.replace(index, t.explicit_type.length(), "${type}");
    }
    
    at->set_Description(s);
    
  }
  
  if (constp)
    at->set_isReadOnly(TRUE);
  
  if (staticp)
    at->set_isClassMember(TRUE);
  
  if (! value.isEmpty())
    at->set_DefaultValue(value);
  
  at->set_Visibility(visibility);
  
  return TRUE;
}


