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

#ifndef ACTIVITYDIALOG_H
#define ACTIVITYDIALOG_H

#include <qwidgetlist.h>
#include <qstringlist.h>
#include <qtabdialog.h>

#include "UmlEnum.h"
#include "BrowserNode.h"

class QPushButton;
class QComboBox;
class QCheckBox;

class LineEdit;
class MultiLineEdit;
class ActivityData;
class KeyValuesTable;
struct InfoData;
class BodyDialog;

struct CondDialog {
  MultiLineEdit * edpre;
  MultiLineEdit * edpost;
  
  void accept(InfoData &);
};

class ActivityDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    bool visit;
    ActivityData * activity;
    QStringList list;
    BrowserNodeList opers;
    LineEdit * edname;
    QComboBox * edstereotype;
    QComboBox * edspecification;
    MultiLineEdit * comment;
    MultiLineEdit * constraint;
    QList<BodyDialog> edits;
    QCheckBox * readonly_cb;
    QCheckBox * singlexec_cb;
    QCheckBox * active_cb;
    CondDialog uml;
    CondDialog cpp;
    CondDialog java;
        
    // User
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
    static void post_edit_description(ActivityDialog * d, QString s);
    static void post_edit_constraint(ActivityDialog * d, QString s);
  
  public:
    ActivityDialog(ActivityData * r);
    virtual ~ActivityDialog();
  
  protected:
    void init_tab(CondDialog &, InfoData & cond, const char * lbl, bool enabled);
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void menu_specification();
    void edit_description();
    void edit_constraint();

    void change_tabs(QWidget *);
};

#endif
