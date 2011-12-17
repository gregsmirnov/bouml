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

#ifndef PACKAGE_H
#define PACKAGE_H

#include <qstringlist.h> 
#include <qstack.h>

#include "Namespace.h"
#include "Class.h"
#include "UmlClass.h"

#include "BrowserNode.h"
#include "ClassContainer.h"

class QPixmap;
class QDir;
class QRegExp;

class UmlPackage;
class BrowserView;
class UmlClass;
class UmlItem;

class Progress;
class QApplication;

class Package : public BrowserNode, public ClassContainer {
  public:
#ifndef REVERSE
    Package(BrowserView * parent, UmlPackage * u);
#endif
    Package(Package * parent, const char * p, const char * n);
  
    virtual bool isa_package() const;
    
    void reverse_file(QCString path, QCString f);
    
    const QCString & get_path() { return path; }
    UmlPackage * get_uml(bool mandatory = TRUE);
#ifdef REVERSE
    void send_dir(bool rec);
#endif
    
    void new_class(Class *);
    
    virtual void compute_type(QCString type, UmlTypeSpec & typespec,
			      Class ** need_object = 0);
    virtual Class * define(const QCString & name, char st);
#ifdef WITH_PHPCAT
    virtual void declare(const QCString &, Class *);
    void restore_children(QDataStream & dts);
    static void restore(QDataStream  & dt, Package *);
#endif
    
#ifndef REVERSE
    virtual QString get_path() const;
    
    virtual void menu();
    virtual void refer(const QString & name);
    
    const QPixmap * pixmap(int) const;
    
    virtual void backup(QDataStream  & dts) const;
    void backup_children(QDataStream  & ts) const;
#endif

    static void init(UmlPackage *, QApplication *);
    static bool scanning() { return scan; };
    static Package * scan_dir();
    static Package * get_root() { return root; };
        
    static void progress_closed();
    
  private:
    UmlPackage * uml;
    QCString path;
    NDict<Class> Undefined;
  
    static bool scan;
    static Package * root;
    static Package * unknown;
    static QRegExp * DirFilter;
    static QRegExp * FileFilter;
    static QString Ext;
    static NDict<Class> classes;
    static NDict<Class> php_classes;
    static NDict<UmlClass> user_classes;
    static Progress * progress;
    static QApplication * app;
    
    static int file_number(QDir & dir, bool rec);
    
    void reverse_toplevel_form(QCString s);
    void reverse_directory(QDir & dir, bool rec);
    
    Package * find(QCString s, bool nohack);
    static Package * package_unknown();
    
    static void update_class_list(QCString pack, UmlItem * container);
    
    Class * declare_if_needed(QCString name, char st);
    Class * new_class(const QCString & name, char st);
    
    void use();
};

#endif
