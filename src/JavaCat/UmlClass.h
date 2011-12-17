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

#ifdef ROUNDTRIP
class Class;
class ClassContainer;
#endif

#ifdef REVERSE
#include <qstringlist.h>
#endif

class UmlClass : public UmlBaseClass {
#ifdef ROUNDTRIP
  private:
    bool created;
    Class * the_class;
#endif
  
  public:
    UmlClass(void * id, const QCString & n);
#ifdef REVERSE
    void need_artifact(const QStringList & imports, bool remove_java_lang,
		       const QStringList & static_imports,
		       const QCString & path, UmlArtifact *& cp);
    
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
    void reorder(QList<UmlItem> & expected_order);
# endif
#endif
    static void manage_generic(QCString & form, UmlTypeSpec & typespec,
			       QCString str_actuals, const char * k);
};

#endif
