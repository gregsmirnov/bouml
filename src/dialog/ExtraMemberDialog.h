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

#ifndef EXTRAMEMBERDIALOG_H
#define EXTRAMEMBERDIALOG_H

#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"
#include "VisibilityGroup.h"

class QComboBox;
class QCheckBox;
class QRadioButton;

class LineEdit;
class MultiLineEdit;
class ExtraMemberData;
class BrowserNode;
class KeyValuesTable;
class BodyDialog;

class ExtraMemberDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    ExtraMemberData * emd;
    QList<BodyDialog> edits;
    
    // uml tab
    LineEdit * edname;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    
    // C++ tab
    QCheckBox * inline_cb;
    MultiLineEdit * edcpp_decl;
    MultiLineEdit * edcpp_def;
    
    // java tab
    MultiLineEdit * edjava_decl;
    
    // php tab
    MultiLineEdit * edphp_decl;
    
    // python tab
    MultiLineEdit * edpython_decl;
    
    // IDL tab
    MultiLineEdit * edidl_decl;
    
    // User tab
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
    static void post_edit_description(ExtraMemberDialog *, QString);
    static void post_edit_cpp_decl(ExtraMemberDialog *, QString);
    static void post_edit_cpp_def(ExtraMemberDialog *, QString);
    static void post_edit_java_decl(ExtraMemberDialog *, QString);
    static void post_edit_php_decl(ExtraMemberDialog *, QString);
    static void post_edit_python_decl(ExtraMemberDialog *, QString);
    static void post_edit_idl_decl(ExtraMemberDialog *, QString);
    
  public:
    ExtraMemberDialog(ExtraMemberData * a);
    virtual ~ExtraMemberDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edit_cpp_decl();
    void edit_cpp_def();
    void edit_java_decl();
    void edit_php_decl();
    void edit_python_decl();
    void edit_idl_decl();
    void edit_description();
};

#endif
