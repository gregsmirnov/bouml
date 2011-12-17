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

#ifndef TOOLDIALOG_H
#define TOOLDIALOG_H

#include <qtabdialog.h>

#include "StringTable.h"
#include "UmlEnum.h"

class QPixmap;

struct ToolColumnDef {
  UmlCode kind;
  QPixmap ** pixmap;
};

class ToolDialog : public QTabDialog {
  Q_OBJECT
    
  public:
    ToolDialog(); 
    virtual ~ToolDialog(); 
    
  private:
    static QSize previous_size;
  
  protected slots:
    virtual void accept();
    virtual void polish();
};

class ToolTable : public StringTable {
  Q_OBJECT
    
  private:
    const ToolColumnDef * cd;
  
  public:
    ToolTable(QWidget * parent, const ToolColumnDef * tcd, unsigned ncols);
  
    virtual void init_row(int r);

    unsigned ntools();
    unsigned update(unsigned rank);
  
  protected slots:
    virtual void button_pressed(int row, int col, int button, const QPoint & mousePos);
};

#endif
