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

#ifndef PACKAGEDIALOG_H
#define  PACKAGEDIALOG_H

#include <qtable.h>
#include <qtabdialog.h>

class QComboBox;
class QCheckBox;
class QPushButton;

class LineEdit;
class MultiLineEdit;
class PackageData;
class BrowserNode;
class KeyValuesTable;
class BodyDialog;

class PackageDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    QWidget * umltab;
    PackageData * pa;
    LineEdit * edname;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    QList<BodyDialog> edits;
    
    // C++
    QWidget * cpptab;
    LineEdit * edcppsrcdir;
    LineEdit * edcpphdir;
    LineEdit * edcppnamespace;
    QPushButton * cppsrcbutton;
    QPushButton * cpphbutton;

    // Java
    QWidget * javatab;
    LineEdit * edjavadir;
    LineEdit * edjavapackage;
    QPushButton * javabutton;

    // Php
    QWidget * phptab;
    LineEdit * edphpdir;
    LineEdit * edphpnamespace;
    QPushButton * phpbutton;

    // Python
    QWidget * pythontab;
    LineEdit * edpythondir;
    LineEdit * edpythonpackage;
    QPushButton * pythonbutton;

    // Idl
    QWidget * idltab;
    LineEdit * edidldir;
    LineEdit * edidlmodule;
    QPushButton * idlbutton;
    
    // Profile
    QWidget * profiletab;
    LineEdit * edmetamodelReference;
    LineEdit * edmetaclassreference;

    // User
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
  public:
    PackageDialog(PackageData * d);
    virtual ~PackageDialog();
  
  protected:
    void browse(LineEdit * ed, QPushButton * button, const char * caption,
		QString root);
    void relative(LineEdit * ed, QPushButton * button, QString root);
    
    static void post_edit_description(PackageDialog * d, QString s);
  
  protected slots:
    virtual void polish();
    virtual void accept();
    void edit_description();
    void edStereotypeActivated(const QString &);
    void cppsrc_browse();
    void cpph_browse();
    void java_browse();
    void php_browse();
    void python_browse();
    void idl_browse();
    void cppsrc_relative();
    void cpph_relative();
    void java_relative();
    void php_relative();
    void python_relative();
    void idl_relative();

    void change_tabs(QWidget *);
};

#endif
