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

#ifndef UMLEXTRACLASSMEMBER_H
#define UMLEXTRACLASSMEMBER_H

#include "UmlBaseExtraClassMember.h"

// This class allows to manage extra class member, mainly defined for C++
// it allows to insert C++ pre-processor directive (even they may be placed
// in the other member definition/declaration), to declare friend
// operation/function etc...
// You can modify it as you want (except the constructor)

class UmlExtraClassMember : public UmlBaseExtraClassMember {
  public:
    UmlExtraClassMember(void * id, const QCString & n)
      : UmlBaseExtraClassMember(id, n) {};
  
    virtual void generate(QTextOStream & f, const QCString & cl_stereotype,
			  QCString indent, BooL & indent_needed,
			  int &, const QCString &);
};

#endif
