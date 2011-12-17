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

#ifndef CLASSINSTANCEDIALOG_H
#define CLASSINSTANCEDIALOG_H

#include <qtabdialog.h>
#include <qstringlist.h>

#include "BrowserNode.h"
#include "MyTable.h"

class QComboBox;
class LineEdit;
class KeyValuesTable;
class ClassInstanceData;
class MultiLineEdit;
class BodyDialog;
struct SlotRel;

class RelTable : public MyTable {
  Q_OBJECT
    
  public:
    RelTable(QWidget * parent, ClassInstanceData * inst, bool visit);
  
    void init_row(const SlotRel & sr, int row, QString a, bool visit);
  
  protected slots:
    virtual void button_pressed(int row, int col, int button, const QPoint & mousePos);
};

class ClassInstanceDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    void init_table();
  
    bool visit;
    ClassInstanceData * inst;
    QStringList list;
    BrowserNodeList nodes;
    LineEdit * edname;
    QComboBox * edtype;
    QComboBox * edstereotype;
    BrowserNodeList attributes;
    BrowserNode * cl_container;
    MultiLineEdit * comment;
    QList<BodyDialog> edits;
    MyTable * atbl;
    RelTable * rtbl;
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
    
    void init_rels();
    
    static void post_edit_description(ClassInstanceDialog *, QString);
  
  public:
    ClassInstanceDialog(ClassInstanceData * i);
    virtual ~ClassInstanceDialog();
  
  protected slots:
    virtual void polish();
    void accept();
    
    void edit_description();
    void type_changed(int);
    void menu_class();
    void update_all_tabs(QWidget *);
};

#endif
