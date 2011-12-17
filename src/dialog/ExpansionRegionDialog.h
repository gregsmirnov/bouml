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

#ifndef EXPANSIONREGIONDIALOG_H
#define EXPANSIONREGIONDIALOG_H





#include <qlist.h>
#include <qtabdialog.h>

class QComboBox;
class QCheckBox;

class LineEdit;
class MultiLineEdit;
class ExpansionRegionData;
class KeyValuesTable;
class BodyDialog;

class ExpansionRegionDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    QWidget * umltab;
    ExpansionRegionData * data;
    LineEdit * edname;
    QComboBox * edstereotype;
    QComboBox * edmode;
    QCheckBox * must_isolate_cb;
    MultiLineEdit * comment;
    KeyValuesTable * kvtable;
    QList<BodyDialog> edits;

    static QSize previous_size;
    
    static void post_edit_description(ExpansionRegionDialog * d, QString s);
      
  public:
    ExpansionRegionDialog(ExpansionRegionData * nd);
    virtual ~ExpansionRegionDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
    void edit_description();

    void change_tabs(QWidget *);
};

#endif
