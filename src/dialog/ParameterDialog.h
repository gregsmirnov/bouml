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

#ifndef PARAMETERDIALOG_H
#define PARAMETERDIALOG_H





#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"

class QComboBox;
class QCheckBox;
class QRadioButton;

class BrowserParameter;
class ParameterData;
class BrowserNode;
class LineEdit;
class MultiLineEdit;
class KeyValuesTable;
class BodyDialog;

class ParameterDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    bool visit;
    ParameterData * param;
    QStringList list;
    BrowserNodeList nodes;
    QList<BodyDialog> edits;
    BrowserNode * view;
    int offset;
    
    // uml tab
    QWidget * umltab;
    LineEdit * edname;
    LineEdit * edinit;
    QComboBox * edtype;
    QComboBox * edstereotype;
    QComboBox * eddir;
    QComboBox * edordering;
    QComboBox * edeffect;
    QComboBox * edmultiplicity;
    LineEdit * edin_state;
    QCheckBox * is_control_cb;
    QCheckBox * unique_cb;
    QRadioButton * standard_rb;
    QRadioButton * exception_rb;
    QRadioButton * stream_rb;
    QWidget * ocltab;
    QWidget * cpptab;
    QWidget * javatab;
    MultiLineEdit * eduml_selection;
    MultiLineEdit * edcpp_selection;
    MultiLineEdit * edjava_selection;
    MultiLineEdit * comment;
    
    // User tab
    KeyValuesTable * kvtable;

    static QSize previous_size;
  
    static void post_edit_init(ParameterDialog *, QString);
    static void post_edit_description(ParameterDialog *, QString);
    static void post_edit_uml_selection(ParameterDialog * d, QString s);
    static void post_edit_cpp_selection(ParameterDialog * d, QString s);
    static void post_edit_java_selection(ParameterDialog * d, QString s);
    
    void init_tab(QWidget *& tab, MultiLineEdit *& ed, const char * v,
		  const char * lbl, const char * sl, bool enabled);
    
  public:
    ParameterDialog(ParameterData * a);
    virtual ~ParameterDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edit_init();
    void edit_description();
    void edit_uml_selection();
    void edit_cpp_selection();
    void edit_java_selection();
    
    void menu_type();

    void change_tabs(QWidget *);
};

#endif
