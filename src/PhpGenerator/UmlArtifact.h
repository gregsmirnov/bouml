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

#include <qasciidict.h> 

#include "UmlBaseArtifact.h"

class UmlPackage;

// This class manages 'artifacts'
// You can modify it as you want (except the constructor)

class UmlArtifact : public UmlBaseArtifact {
  private:
    bool managed;
    
    static UmlArtifact * current;
    static UmlPackage * package_of_generated_artifact;
    
    static bool must_be_saved(const char * path, const char * new_contains);
  
    void generate_text();

  public:
    UmlArtifact(void * id, const QCString & n)
      : UmlBaseArtifact(id, n), managed(FALSE) {};
    
    virtual void generate();
    
    static UmlArtifact * generated_one();
    static UmlPackage * generation_package();
};

#endif
