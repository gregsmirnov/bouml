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

#ifndef UMLCLASSITEM_H
#define UMLCLASSITEM_H

#include <qlist.h>

#include "UmlBaseClassItem.h"

class QTextOStream;
class CppRefType;

class UmlClassItem : public UmlBaseClassItem {
  public:
    UmlClassItem(void * id, const QCString & n)
      : UmlBaseClassItem(id, n) {};
  
    static void remove_comments(QCString & s);
    static void remove_arrays(QCString & s);
    static void remove_preprocessor(QCString & s);

    virtual void generate_decl(QTextOStream & f, const QCString & cl_stereotype,
			       QCString indent, bool = FALSE) = 0;
};

#endif
