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

#ifndef UMLRELATION_H
#define UMLRELATION_H

#include "UmlBaseRelation.h"

class Class;

class UmlRelation : public UmlBaseRelation {
  public:
    UmlRelation(void * id, const QCString & n)
      : UmlBaseRelation(id, n) {
    };  
  
    static bool new_one(Class * container, const QCString & name,
			UmlClass * dest, const QCString & modifier,
			const QCString & pretype, const QCString & array,
			const QCString & typeform, aVisibility visibility,
			bool staticp, bool constp, bool mutablep, bool volatilep,
			const QCString & value,	QCString comment,
			QCString description
#ifdef ROUNDTRIP
			, bool roundtrip, QList<UmlItem> & expected_order
#endif
			);
    static bool new_friend(UmlClass * from, UmlClass * to);
    
#ifdef ROUNDTRIP
    void set_unidir();
    static bool new_friend(Class * from, UmlClass * to,
			   QList<UmlItem> & expected_order);
#endif
};

#endif
