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

#ifndef UMLCLASS_H
#define UMLCLASS_H

#include <qlist.h>

#include "UmlBaseClass.h"
#include "UmlTypeSpec.h"

class QTextOStream;

class UmlPackage;
class UmlClass;

class UmlClass : public UmlBaseClass {
  private:
    bool managed;
    
  public:
    UmlClass(void * id, const QCString & n)
      : UmlBaseClass(id, n), managed(FALSE) {};
  
    QCString python_stereotype();
    
    virtual void generate();
    virtual void generate(QTextOStream & f, const QCString & cl_stereotype,
			  QCString indent, BooL & indent_needed,
			  int & enum_item_rank, const QCString & self);    
    
    void generate_imports(QTextOStream & f, QCString & made);
    void generate_import(QTextOStream & f, UmlArtifact * using_art, bool from, QCString & made);
    void generate(QTextOStream &, QCString indent, BooL & indent_needed);
    void generate_instance_att_rel(QTextOStream & f, QCString indent,
				   BooL & indent_needed, QCString self);
    
    UmlArtifact * assocArtifact();

    void write(QTextOStream &);
    static void write(QTextOStream &, const UmlTypeSpec &);
};

#endif
