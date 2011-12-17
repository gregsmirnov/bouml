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

#ifndef UMLITEM_H
#define UMLITEM_H

#include "UmlBaseItem.h"

class QTextOStream;
class UmlPackage;

class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n)
      : UmlBaseItem(id, n) {
    };
    virtual ~UmlItem();
    
    virtual void generate();
    virtual UmlPackage * package();
    void manage_comment(const char *& p, const char *& pp);
    void manage_description(const char *& p, const char *& pp);  
    void manage_docstring(const char *& p, const char *& pp, BooL & indent_needed,
			  QCString & indent, QCString & saved_indent);  
    void manage_alias(const char *& p, QTextOStream & ts,
		      QCString indent, BooL & indent_needed);
};

#endif
