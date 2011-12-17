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

#ifndef CLASS_H
#define CLASS_H

#include "BrowserNode.h"
#include "ClassContainer.h"
#include "UmlClass.h"
#include "aRelationKind.h"

class Package;
class UmlArtifact;
#ifndef REVERSE
class QPixmap;
class QPainter;
class QColorGroup;
#endif

class Class : public BrowserNode {
  protected:
    QCString filename;
    QCString its_namespace;
    UmlClass * uml;
    char stereotype;	// 'c' : class, 'i' : interface, '@' @interface, 'e' : enum
    bool abstractp;
    bool reversedp;
#ifdef REVERSE
    bool from_lib;
#else
    bool description_updatedp;
    QCString description;
    
    static QList<Class> Historic;
#endif
    
    bool manage_extends(ClassContainer * container);
    bool manage_implements(ClassContainer * container, aRelationKind k);
    bool add_inherit(aRelationKind k, UmlTypeSpec & typespec);
    bool manage_member(QCString s);
    void set_description(const char * p);
    
#ifndef REVERSE
    virtual void activate();
    void manage_historic();
#endif
    
  public:
    Class(Package * parent, const char * n, char st);
  
    void compute_type(QCString type, UmlTypeSpec & typespec,
		      Class ** need_object = 0);
    const QCString & get_namespace() const { return its_namespace; }
    UmlClass * get_uml();
    bool reversed() const { return reversedp; };
    bool already_in_bouml();
    virtual QString get_path() const;
#ifndef REVERSE
    const QCString & get_description() const { return description; };
    
    virtual void selected();
    virtual void activated();
    virtual void menu();
    virtual void refer(const QString & name);

    const QPixmap * pixmap(int) const;
    void paintCell(QPainter * p, const QColorGroup & cg, int column,
		   int width, int alignment);

        
    virtual void backup(QDataStream & dt) const;
#endif
    
#ifdef WITH_PHPCAT
    static void restore(QDataStream & dt, char c, Package * p);
#endif
    
    static bool reverse(Package * container, QCString stereotype,
			bool abstractp, bool finalp, 
			QCString & f, UmlArtifact * art);
    
#ifndef REVERSE
    static void historic_back();
    static void historic_forward();
#endif
};

#endif
