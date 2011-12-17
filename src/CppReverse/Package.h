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

#include <qdict.h>
#include <qlist.h>

#include "BrowserNode.h"
#include "ClassContainer.h"

class QDir;
class QFileInfo;
class QRegExp;
class UmlPackage;
class QApplication;

class Package : public BrowserNode, public ClassContainer {
  public:
#ifndef REVERSE
    Package(BrowserView * parent, UmlPackage * u);
#endif
    Package(Package * parent, QString p, QString n);
    virtual ~Package();	// just to not have warning
    
#ifdef ROUNDTRIP
    Package(Package * parent, UmlPackage * pk);
    
    virtual Class * upload_define(UmlClass *);
    void own(UmlArtifact *);
    void reverse(UmlArtifact *);
#endif
    void reverse_variable(const QCString & name);
    
    Class * declare_if_needed(const QCString & name,
			      QCString stereotype = 0);
    virtual Class * define(const QCString & name, QCString stereotype = 0);
    virtual void declare_if_needed(QCString name, Class * cl);
    virtual void define(QCString name, Class * cl);
    virtual bool find_type(QCString type, UmlTypeSpec & typespec);
    virtual void declaration(const QCString & name, const QCString & stereotype,
			     const QCString & decl
#ifdef ROUNDTRIP
			     , bool roundtrip, QList<UmlItem> & expected_order
#endif
			     );

    virtual bool isa_package() const;
    const QCString & get_h_path() const { return h_path; };
    const QCString & get_src_path() const { return src_path; };
    UmlPackage * get_uml(bool mandatory = TRUE);
#ifndef REVERSE
    virtual void menu();
    virtual void refer(const QString & name);
    
    const QPixmap * pixmap(int) const;
    
    virtual void backup(QDataStream  & dts) const;
    void backup_children(QDataStream  & ts) const;
#endif
    void restore_children(QDataStream & dts);
    static void restore(QDataStream  & dt, Package *);
    
    static void init(UmlPackage *, QApplication *);
    static bool scanning() { return Scan; };
    static void set_step(int s, int n);
#ifdef ROUNDTRIP
    void scan_dir(int & n);
    void send_dir(int n);
    void accept_roundtrip_root(bool h);
#else
    static void scan_dirs(int & n);
    static void send_dirs(int n, bool rec);
#endif
    
    static const QCString & get_fname() { return fname; }
#ifdef ROUNDTRIP
    static UmlArtifact * get_artifact() { return artfct; }
#endif
    
    //static Package * unknown();
    
  private:
    UmlPackage * uml;
    QCString namespace_;
    QCString h_path;	// empty or finish by a /
    QCString src_path;	// empty or finish by a /
    
    static QApplication * app;
    static QList<Package> Choozen;
    static int Nfiles;
    static bool Scan;
    static Package * Root;
    static QRegExp * DirFilter;
    static QRegExp * FileFilter;
    //static Package * Unknown;
    static QValueList<FormalParameterList> Formals;
    static QList<UmlClass> UsedClasses;
    
    static NDict<Class> Declared;
    static NDict<Class> Defined;
    
    static QCString fname;	// without extension, empty for non header file
#ifdef ROUNDTRIP
    static UmlArtifact * artfct; // currently roundtriped artifact
#endif
    
    void reverse_directory(QString path, bool rec, QString ext, bool h);
#ifdef ROUNDTRIP
    void reverse_file(QCString f, UmlArtifact * art, bool h);
#else
    void reverse_file(QCString f);
#endif    
    void reverse_toplevel_forms(QCString f, bool sub_block);
    void reverse_toplevel_form(QCString f, QCString s);
        
    Class * new_class(const QCString & name, const QCString & stereotype,
		      bool declaration);

    void set_namespace(QCString s);
    Package * find(QFileInfo * di);

#ifdef ROUNDTRIP
    int count_file_number();
    void scan_dir();
    void send_dir();
#endif
    
    static int file_number(QString path, bool rec, const char * ext);
};

#endif
