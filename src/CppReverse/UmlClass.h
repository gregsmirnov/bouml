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

#include "UmlBaseClass.h"
#include "UmlFormalParameter.h"

class ClassContainer;
#ifdef ROUNDTRIP
class Class;
class UmlRelation;
class UmlExtraClassMember;
#endif

typedef QValueList<UmlFormalParameter> FormalParameterList;

class UmlClass : public UmlBaseClass {
#ifdef ROUNDTRIP
  private:
    bool created;
    Class * the_class;
#endif
  
  public:
    UmlClass(void * id, const QCString & n);
  
    bool manage_inherit(ClassContainer * pack, 
			const QValueList<FormalParameterList> & tmplt
#ifdef REVERSE
			, bool libp
# ifdef ROUNDTRIP
			, bool roundtrip, QList<UmlItem> & expected_order
			, bool container_roundtrip, QList<UmlItem> & container_expected_order
# endif
#endif
			);
    bool get_actuals(UmlClass * mother, ClassContainer * container, 
		     const QValueList<FormalParameterList> & formals
#ifdef ROUNDTRIP
		     , bool roundtrip
#endif
		     );
    
    void set_under_construction(bool y, bool rec = FALSE);
    bool inside_its_definition();
    bool is_itself(QCString t);
    
    static void clear_usings() { Usings.clear(); }
    void using_it() { Usings.replace(name(), this); }
    static UmlClass * used(const char * n) { return Usings[n]; }
    static void save_using_scope() {
      UsingScope.prepend(Usings);
    }
    static void restore_using_scope();

#ifdef REVERSE
    void need_artifact(const QCString & nmsp);
    virtual bool need_source();
    
# ifdef ROUNDTRIP
    virtual void upload(ClassContainer * cnt);
    virtual bool set_roundtrip_expected();
    virtual void mark_useless(QList<UmlItem> & l);
    virtual void scan_it(int & n);
    virtual void send_it(int n);
    bool is_created() const { return created; }
    void set_created() { created = TRUE; }
    Class * get_class() const { return the_class; }
    UmlItem * search_for_att_rel(const QCString & name);
    UmlExtraClassMember * search_for_extra(const QCString & name, const QCString & decl);
    UmlRelation * search_for_inherit(UmlClass * mother);
    void reorder(QList<UmlItem> & expected_order);
# endif
#endif
    
    private:
      UmlClass * auxilarily_typedef(const QCString & base
#ifdef REVERSE
				    , bool libp
# ifdef ROUNDTRIP
				    , bool container_roundtrip
				    , QList<UmlItem> & container_expected_order
# endif
#endif
				    );

      static QList<UmlClass> UnderConstruction;
      static QDict<UmlClass> Usings;
      static QValueList<QDict<UmlClass> > UsingScope;
};

#endif
