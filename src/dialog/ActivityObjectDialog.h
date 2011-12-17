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

#ifndef ACTIVITYOBJECTDIALOG_H
#define ACTIVITYOBJECTDIALOG_H





#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"

class QComboBox;
class QCheckBox;
class QRadioButton;

class BrowserActivityObject;
class ActivityObjectData;
class BrowserNode;
class LineEdit;
class MultiLineEdit;
class KeyValuesTable;
class BodyDialog;

class ActivityObjectDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    bool visit;
    ActivityObjectData * data;
    QStringList list;
    BrowserNodeList nodes;
    QList<BodyDialog> edits;
    int offset;
    BrowserNode * view;
    
    QWidget * umltab;
    QWidget * ocltab;
    QWidget * cpptab;
    QWidget * javatab;

    // uml tab
    LineEdit * edname;
    QComboBox * edtype;
    QComboBox * edstereotype;
    QComboBox * edordering;
    QComboBox * edmultiplicity;
    QCheckBox * is_control_cb;
    LineEdit * edin_state;
    MultiLineEdit * eduml_selection;
    MultiLineEdit * edcpp_selection;
    MultiLineEdit * edjava_selection;
    MultiLineEdit * comment;
    
    // User tab
    KeyValuesTable * kvtable;

    static QSize previous_size;
  
    static void post_edit_description(ActivityObjectDialog *, QString);
    static void post_edit_uml_selection(ActivityObjectDialog * d, QString s);
    static void post_edit_cpp_selection(ActivityObjectDialog * d, QString s);
    static void post_edit_java_selection(ActivityObjectDialog * d, QString s);
    
    void init_tab(QWidget *& w, MultiLineEdit *& ed, const char * v,
		  const char * lbl, const char * sl, bool enabled);
    
  public:
    ActivityObjectDialog(ActivityObjectData * a, const char * what,
			 QStringList & st);
    virtual ~ActivityObjectDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edit_description();
    void edit_uml_selection();
    void edit_cpp_selection();
    void edit_java_selection();
    void menu_type();

    void change_tabs(QWidget *);
};

#endif
