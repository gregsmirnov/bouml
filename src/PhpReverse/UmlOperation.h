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

#ifndef UMLOPERATION_H
#define UMLOPERATION_H

#include "UmlBaseOperation.h"
#include "UmlClass.h"

class Class;

class UmlOperation : public UmlBaseOperation {
  private:
    static bool read_param(Class * container, unsigned rank,
			   UmlParameter & param, QCString & decl, bool bypass);
  
  public:
    UmlOperation(void * id, const QCString & n)
      : UmlBaseOperation(id, n) {};
	
    static bool new_one(Class * container, aVisibility visibility,
			bool finalp, bool abstractp, bool staticp,
			QCString comment, QCString description);

    static void skip_body(int level = 0);
    static QCString skip_expr(int level = 0);
};

#endif
