// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Constraintkit.
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

#ifndef CONSTRAINTDIALOG_H
#define CONSTRAINTDIALOG_H

#include <qdialog.h>
#include <qtable.h>

#include "StringTable.h"
#include "UmlEnum.h"

class QCheckBox;
class ConstraintCanvas;
class ConstraintTable;

class ConstraintDialog : public QDialog {
  Q_OBJECT
 
  private:
    ConstraintCanvas * constraint;
    ConstraintTable * table;
    QCheckBox * cb_visible;
 
    static QSize previous_size;
    
  public:
    ConstraintDialog(ConstraintCanvas * c); 
    virtual ~ConstraintDialog(); 
    
  protected slots:
    void show_all();
    void hide_all();
    void hide_inherited();
    virtual void accept();
    virtual void polish();
};

class ConstraintTable : public QTable {
  Q_OBJECT
    
  public:
    ConstraintTable(QWidget * parent, ConstraintCanvas * c);
    void show_all();
    void hide_all();
    void hide_inherited(ConstraintCanvas * c);
    void update(ConstraintCanvas * c);
  
  protected slots:
    virtual void button_pressed(int row, int col, int button, const QPoint & mousePos);
};

#endif
