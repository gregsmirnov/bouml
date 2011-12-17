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

#ifndef PARAMETERSETDIALOG_H
#define PARAMETERSETDIALOG_H





#include <qptrdict.h>
#include <qmap.h>
#include <qtabdialog.h>

#include "BrowserParameterSet.h"

class QComboBox;
class QListBox;
class QVBox;
class LineEdit;
class MultiLineEdit;
class ParameterSetData;
class KeyValuesTable;
class BodyDialog;

class ParameterSetDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    QWidget * umltab;
    ParameterSetData * data;
    LineEdit * edname;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    QList<BodyDialog> edits;
    
    // associated classes
    QListBox * lb_available;
    QListBox * lb_member;
        
    // user
    KeyValuesTable * kvtable;
    
    static QSize previous_size;
    
    void init_uml_tab();
    void init_pins_tab();
      
    static void post_edit_description(ParameterSetDialog * d, QString s);
    
  public:
    ParameterSetDialog(ParameterSetData * nd);
    virtual ~ParameterSetDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
    void edit_description();
    void associate_cls();
    void unassociate_cls();

    void change_tabs(QWidget *);
};

#endif
