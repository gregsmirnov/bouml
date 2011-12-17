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

#ifndef ARTIFACTDIALOG_H
#define ARTIFACTDIALOG_H





#include <qptrdict.h>
#include <qmap.h>
#include <qtabdialog.h>

#include "BrowserArtifact.h"

class QComboBox;
class QSplitter;
class QListBox;
class QVBox;
class LineEdit;
class MultiLineEdit;
class ArtifactData;
class KeyValuesTable;
class FormalParamsTable;
class ActualParamsTable;
class BodyDialog;

class ArtifactDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    ArtifactData * data;
    QWidget * uml_page;
    LineEdit * edname;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    QMap<BrowserClass *, FormalParamsTable *> formals;	// to generate the associated
    QMap<BrowserClass *, ActualParamsTable *> actuals;	// classes definition
    QList<BodyDialog> edits;
    
    // c++
    unsigned n_cpp;
    QSplitter * cpp_h_content_page;
    MultiLineEdit * edcpp_h_content;
    MultiLineEdit * showcpp_h_content;
    QSplitter * cpp_src_content_page;
    MultiLineEdit * edcpp_src_content;
    MultiLineEdit * showcpp_src_content;
    
    // java
    unsigned n_java;
    QSplitter * java_content_page;
    MultiLineEdit * edjava_content;
    MultiLineEdit * showjava_content;
    
    // php
    unsigned n_php;
    QSplitter * php_content_page;
    MultiLineEdit * edphp_content;
    MultiLineEdit * showphp_content;
    
    // python
    unsigned n_python;
    QSplitter * python_content_page;
    MultiLineEdit * edpython_content;
    MultiLineEdit * showpython_content;
    
    // idl
    unsigned n_idl;
    QSplitter * idl_content_page;
    MultiLineEdit * edidl_content;
    MultiLineEdit * showidl_content;
    
    // associated classes
    QVBox * cl_assoc_page;
    QListBox * lb_cl_available;
    QListBox * lb_cl_associated;
    
    // associated artifacts
    bool lb_art_initialized;
    QPtrDict<BrowserArtifact> art_associated;
    QVBox * art_assoc_page;
    QComboBox * stereotypefilter;
    QListBox * lb_art_available;
    QListBox * lb_art_associated;
    
    // user
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
    
    void init_uml_tab();
    void init_cpp_tab();
    void init_java_tab();
    void init_php_tab();
    void init_python_tab();
    void init_idl_tab();
    void init_assoc_classes_tab();
    void init_assoc_artifacts_tab();
  
    void compute_cpp_namespace(QString & nasp, QString & nasp_start, QString & nasp_end);
    QString compute_java_package();
    QString compute_php_namespace();
    void compute_idl_module(QString & mod, QString & mod_start, QString & mod_end);
    
    static void post_edit_description(ArtifactDialog * d, QString s);
    static void post_cpp_edit_h(ArtifactDialog * d, QString s);
    static void post_cpp_edit_src(ArtifactDialog * d, QString s);
    static void post_java_edit(ArtifactDialog * d, QString s);
    static void post_php_edit(ArtifactDialog * d, QString s);
    static void post_python_edit(ArtifactDialog * d, QString s);
    static void post_idl_edit(ArtifactDialog * d, QString s);
    
  public:
    ArtifactDialog(ArtifactData * nd);
    virtual ~ArtifactDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
    void default_description();
    void edit_description();
    void edStereotypeActivated(const QString &);
    void update_tab(QWidget *);
    void cpp_edit_h();
    void cpp_update_h();
    void cpp_default_h();
    void cpp_unmapped_h();
    void cpp_edit_src();
    void cpp_update_src();
    void cpp_default_src();
    void cpp_unmapped_src();
    void java_edit();
    void java_update_src();
    void java_default_src();
    void java_unmapped_src();
    void php_edit();
    void php_update_src();
    void php_default_src();
    void php_unmapped_src();
    void python_edit();
    void python_update_src();
    void python_default_src();
    void python_unmapped_src();
    void idl_edit();
    void idl_update_src();
    void idl_default_src();
    void idl_unmapped_src();
    void associate_cls();
    void unassociate_cls();
    void move_cls_up();
    void move_cls_down();
    void associate_arts();
    void unassociate_arts();
    void stereotypeFilterActivated(const QString &);
};

#endif
