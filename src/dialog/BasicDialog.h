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

#ifndef BASICDIALOG_H
#define BASICDIALOG_H





#include <qlist.h>
#include <qtabdialog.h>

class QComboBox;
class LineEdit;
class MultiLineEdit;
class BasicData;
class KeyValuesTable;
class BodyDialog;

class BasicDialog : public QTabDialog {
  Q_OBJECT
    
  protected:
    BasicData * data;
    LineEdit * edname;
    QComboBox * edstereotype;
    MultiLineEdit * comment;
    KeyValuesTable * kvtable;
    QSize & previous_size;
    QList<BodyDialog> edits;
    
    static void post_edit_description(BasicDialog * d, QString s);
      
  public:
    BasicDialog(BasicData * nd, QString s,
		const QStringList & default_stereotypes,
		QSize &, bool unnamed = FALSE);
    virtual ~BasicDialog();
  
  protected slots:
    virtual void polish();
    virtual void accept();
    void edit_description();
};

#endif
