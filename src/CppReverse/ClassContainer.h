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

#ifndef CLASSCONTAINER
#define CLASSCONTAINER

#include "UmlClass.h"
#include "Namespace.h"

class Class;
class UmlTypeSpec;

class ClassContainer {
  public:
    virtual ~ClassContainer();	// just to not have warning
  
    virtual Class * declare_if_needed(const QCString & name,
				      QCString stereotype = 0) = 0;
    virtual Class * define(const QCString & name, QCString stereotype = 0) = 0;
    virtual void declare_if_needed(QCString name, Class * cl) = 0;
    virtual void define(QCString name, Class * cl) = 0;
    virtual Class * new_class(const QCString & name,
			      const QCString & stereotype,
			      bool declaration) = 0;
    virtual bool find_type(QCString type, UmlTypeSpec & typespec) = 0;
    virtual void declaration(const QCString & name, const QCString & stereotype,
			     const QCString & decl
#ifdef ROUNDTRIP
			     , bool roundtrip, QList<UmlItem> & expected_order
#endif
			     ) = 0;
    
    void compute_type(QCString type, UmlTypeSpec & typespec,
		      QCString & typeform, bool get_first_template_actual = FALSE,
		      const QValueList<FormalParameterList> & tmplts = empty);
    bool find_type(QCString type, UmlTypeSpec & typespec,
		   NDict<Class> & defined);
#ifdef ROUNDTRIP
    virtual Class * upload_define(UmlClass *) = 0;
#endif
    
    static bool get_template(FormalParameterList & tmplt);
    
  protected:
    Class * declare_if_needed(const QCString & name,
			      const QCString & stereotype,
			      const FormalParameterList & formals,
			      NDict<Class> & declared, 
			      NDict<Class> & defined);
    Class * define(const QCString & name, const QCString & stereotype,
		   NDict<Class> & declared, NDict<Class> & defined);
    
    static QValueList<FormalParameterList> empty;
};

#endif
