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





#include <qpopupmenu.h> 
#include <qcursor.h>

#include "StringTable.h"
#include "translate.h"

StringTable::StringTable(int numRows, int numCols, QWidget * parent, bool visit)
    : MyTable(numRows, numCols, parent) {
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  
  copy = new QString[numCols];
  
  if (! visit) {
    connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	    this, SLOT(button_pressed(int, int, int, const QPoint &)));
    connect(this, SIGNAL(valueChanged(int, int)),
	    this, SLOT(value_changed(int, int)));
  }
}

StringTable::~StringTable() {
  delete [] copy;
}

void StringTable::activateNextCell() {
  int row = currentRow();
  int col = currentColumn();
    
  if (col ==  (numCols() - 2)) {
    // goto next line
    if (++row == numRows()) {
      // adds a new line
      setNumRows(row + 1);
      init_row(row);
      setText(row, numCols() - 1, QString::null);
    }
    setCurrentCell(row, 0);
  }
  else
    setCurrentCell(row, col + 1);
}

void StringTable::value_changed(int row, int col) {
  if ((row == (numRows() - 1)) && (col != 0) && !text(row, col).isEmpty())
    insert_row_after(row);
}

void StringTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == numCols() - 1) {
    QString s;
    QPopupMenu m;
    
    s.sprintf("%d", row + 1);
    m.insertItem(TR("row %1", s), -1);
    m.insertSeparator();
    m.insertItem(TR("Insert row before"), 0);
    m.insertItem(TR("Insert row after"), 1);
    m.insertSeparator();
    m.insertItem(TR("Delete row"), 2);
    m.insertSeparator();
    m.insertItem(TR("Copy row"), 3);
    m.insertItem(TR("Cut row"), 4);
    m.insertItem(TR("Paste row"), 5);
    m.insertSeparator();

    QPopupMenu mv;
    int rank;
    
    for (rank = 0; rank != numRows(); rank += 1)
      if (rank != row)
	mv.insertItem(QString::number(rank + 1), 10 + rank);
    
    m.insertItem(TR("Move row"), &mv);
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      break;
    case 1:
      insert_row_after(row);
      break;
    case 2:
      delete_row(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      break;
    case 5:
      paste_row(row);
      break;
    default:
      if (rank >= 10)
	move_row(row, rank - 10);
      break;
    }
  }
}

void StringTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    int col;
    
    for (col = 0; col <= numCols() - 1; col += 1)
      setText(index, col, text(index - 1, col));
  }
  
  init_row(row);
  setText(row, numCols() - 1, QString::null);
 }

void StringTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    int col;
    
    for (col = 0; col <= numCols() - 1; col += 1)
      setText(index, col, text(index - 1, col));
  }
  
  init_row(row + 1);
  setText(row + 1, numCols() - 1, QString::null);
 }

void StringTable::delete_row(int row) {
  int n = numRows();
  int index;

  clearCellWidget(row, 1);
    
  if (n == 1) {
    // the alone line : empty it
    init_row(0);
    setText(0, numCols() - 1, QString::null);
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      int col;
    
      for (col = 0; col <= numCols() - 1; col += 1)
	setText(index, col, text(index + 1, col));
    }
    setNumRows(n - 1);
  }
}

void StringTable::copy_row(int row) {
  int col;
    
  for (col = 0; col <= numCols() - 1; col += 1)
    copy[col] = text(row, col);
}

void StringTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void StringTable::paste_row(int row) {
  int col;
    
  for (col = 0; col <= numCols() - 1; col += 1)
    setText(row, col, copy[col]);

  if (row == (numRows() - 1))
    insert_row_after(row);
}

void StringTable::move_row(int from, int to) {
  QString * save = new QString[numCols()];
  int col;
    
  for (col = 0; col <= numCols() - 1; col += 1)
    save[col] = copy[col];
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  for (col = 0; col <= numCols() - 1; col += 1)
    copy[col] = save[col];
  
  delete [] save;
}
