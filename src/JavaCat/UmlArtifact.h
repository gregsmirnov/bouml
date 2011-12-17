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

#ifndef UMLARTIFACT_H
#define UMLARTIFACT_H

#include "UmlBaseArtifact.h"

// This class manages 'artifact'
// You can modify it as you want (except the constructor)

class UmlArtifact : public UmlBaseArtifact {
#ifdef ROUNDTRIP
  private:
    bool roundtrip_expected;
    bool useless;
    bool fully_updated;
    
    static bool has_roundtrip_expected;
#endif
  public:
    UmlArtifact(void * id, const QCString & n)
      : UmlBaseArtifact(id, n)
#ifdef ROUNDTRIP
	, roundtrip_expected(FALSE), useless(FALSE), fully_updated(FALSE)
#endif
      {}
  
#ifdef ROUNDTRIP
    virtual bool set_roundtrip_expected();
    virtual void scan_it(int & n);
    virtual void send_it(int n);
    virtual void mark_useless(QList<UmlItem> & l);
    bool set_roundtrip_expected_for_class();
    bool is_roundtrip_expected() const { return roundtrip_expected; }
    bool is_fully_updated() const { return fully_updated; }
    void set_usefull() { useless = FALSE; }
    
    static bool is_roundtrip_usefull();
#endif

};

#endif
