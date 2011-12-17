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





#include "MyTable.h"
#include "strutil.h"

// TableItem is defined to redefine alignment() because the default
// definition calls QString::to[U]Long which does not properly
// manage the empty but not null QString (ie QString(""))

int TableItem::alignment() const {
  return Qt::AlignLeft | Qt::AlignVCenter;
}

// to use TableItem rather than QTableItem

void MyTable::setText(int row, int col, const QString & text) {
  QTableItem * itm = item(row, col);
  
  if (itm != 0) {
    itm->setText(text);
    updateCell(row, col);
  }
  else
    setItem(row, col, new TableItem(this, QTableItem::OnTyping, text));
}

// force end of edition

void MyTable::forceUpdateCells() {
  for (int row = 0; row != numRows(); row += 1) {
    for (int col = 0; col != numCols(); col += 1) {
      QWidget * w = cellWidget(row, col);
      
      if (w != 0) {
	QString s = item(row, col)->text();
	
	item(row, col)->setContentFromEditor(w);
	if (item(row, col)->text() != s)
	  emit valueChanged(row, col);
	
	// warning : may have other edited cells
      }
    }
  }
}
