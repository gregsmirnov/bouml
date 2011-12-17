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

#ifndef UMLATTRIBUTE_H
#define UMLATTRIBUTE_H

class QTextOStream;

#include "UmlBaseAttribute.h"

class UmlAttribute : public UmlBaseAttribute {
  public:
    UmlAttribute(void * id, const QCString & n)
      :  UmlBaseAttribute(id, n) {};

    virtual void generate(QTextOStream & f, const QCString & cl_stereotype,
			  QCString indent);
    virtual void generate_enum_pattern_item(QTextOStream & f, int & current_value,
					    const QCString & class_name,
					    QCString indent);
    virtual void generate_enum_pattern_case(QTextOStream & f, QCString indent);
    virtual void generate_enum_item(QTextOStream & f, QCString indent, BooL & first);
    virtual void generate_enum_member(QTextOStream & f, QCString indent);
    
  private:
    void generate(QTextOStream & f, const QCString & cl_stereotype,
		  QCString indent, bool enumitem);

};

#endif
