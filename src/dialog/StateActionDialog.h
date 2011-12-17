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

#ifndef STATEACTIONDIALOG_H
#define STATEACTIONDIALOG_H

#include <qwidgetlist.h>
#include <qstringlist.h>
#include <qtabdialog.h>

#include "BrowserNode.h"

class QPushButton;
class QComboBox;

class LineEdit;
class MultiLineEdit;
class StateActionData;
class BrowserNode;
class KeyValuesTable;
class BodyDialog;

class StateActionDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    bool visit;
    StateActionData * action;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    QList<BodyDialog> edits;
    
    QWidget * umltab;
    QWidget * cpptab;
    QWidget * javatab;
    MultiLineEdit * uml;
    MultiLineEdit * cpp;
    MultiLineEdit * java;
        
    // User
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
  
    static void post_edit_description(StateActionDialog * d, QString s);
  
  public:
    StateActionDialog(StateActionData * r);
    virtual ~StateActionDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
  
    void edit_description();

    void change_tabs(QWidget *);
};

#endif
