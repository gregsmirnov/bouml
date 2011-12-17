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

#ifndef STRINGTABLE
#define STRINGTABLE

#include "MyTable.h"

class StringTable : public MyTable {
  Q_OBJECT

  protected:
    QString * copy;		// buffer for copy/cut/paste

  public:
    StringTable(int numRows, int numCols, QWidget * parent, bool visit);
    virtual ~StringTable();
  
    virtual void init_row(int index) = 0;
  
  protected:
    virtual void activateNextCell();
  
    void insert_row_before(int row);
    void insert_row_after(int row);
    void delete_row(int row);
    void copy_row(int row);
    void cut_row(int row);
    void paste_row(int row);
    void move_row(int from, int to);
  
  protected slots:
    virtual void button_pressed(int row, int col, int button, const QPoint & mousePos);
    void value_changed(int row, int col);
};

#endif
