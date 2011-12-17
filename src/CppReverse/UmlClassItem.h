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

class UmlClassItem : public UmlBaseClassItem {
#ifdef ROUNDTRIP
  private:
    bool roundtrip_expected;
    bool useless;
#endif
  public:
    UmlClassItem(void * id, const QCString & n)
      : UmlBaseClassItem(id, n)
#ifdef ROUNDTRIP
	, roundtrip_expected(FALSE), useless(FALSE)
#endif
	  {}

#ifdef REVERSE
    virtual bool need_source() = 0;
  
# ifdef ROUNDTRIP
    bool is_roundtrip_expected() const { return roundtrip_expected; }
    bool is_useless() const { return useless; }
    void set_usefull(bool y = TRUE) { useless = !y; }
    virtual bool set_roundtrip_expected();
    virtual void mark_useless(QList<UmlItem> & l);
# endif
#endif
};

#endif
