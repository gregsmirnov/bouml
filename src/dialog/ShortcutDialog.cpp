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





#include <stdio.h>

#include <qcursor.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h> 
#include <qlayout.h>
#include <qpopupmenu.h>

#include "ShortcutDialog.h"
#include "Shortcut.h"
#include "Tool.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "ComboItem.h"
#include "translate.h"

QSize ShortcutDialog::previous_size;

ShortcutDialog::ShortcutDialog() : QTabDialog(0, 0, TRUE) {
  setOkButton(TR("OK"));
  setCancelButton(TR("Cancel"));
  setCaption(TR("Shortcut dialog"));

  // count number of entries
  
  QStringList tools = Tool::all_display();
  const QMap<QString, QString> & shortcuts = Shortcut::shortcuts();
  QMap<QString, QString>::ConstIterator iter;
  int ntools = 0;
  int ncmds = 0;

  for (iter = shortcuts.begin(); iter != shortcuts.end(); ++iter) {
    if (tools.findIndex(iter.data()) != -1)
      ntools += 1;
    else
      ncmds += 1;
  }

  // tab for command
  
  QVBox * vtab;
    
  vtab = new QVBox(this);
  vtab->setSpacing(5);
  vtab->setMargin(5);
  
  (new QLabel(TR("Here are the shortcuts to do a command (menu entry)"), vtab))
    ->setAlignment(::Qt::AlignHCenter);

#ifdef __APPLE__
  (new QLabel(TR("Note : sometimes the key 'Alt' is named 'Option'"), vtab))
    ->setAlignment(::Qt::AlignHCenter);
#endif  
  
  cmd_table = new ShortcutTable(vtab, FALSE, ncmds);
  
  addTab(vtab, TR("Command"));
  
  // tab for tool
  
  vtab = new QVBox(this);
  vtab->setSpacing(5);
  vtab->setMargin(5);
  
  (new QLabel(TR("Here are the shortcuts to call a tool (plug-out)"), vtab))
    ->setAlignment(::Qt::AlignHCenter);

#ifdef __APPLE__
  (new QLabel(TR("Note : sometimes the key 'Alt' is named 'Option'"), vtab))
    ->setAlignment(::Qt::AlignHCenter);
  
#endif  
  tool_table = new ShortcutTable(vtab, TRUE, ntools);
  
  addTab(vtab, TR("Tool"));
}

ShortcutDialog::~ShortcutDialog() {
  previous_size = size();
}

void ShortcutDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void ShortcutDialog::accept() {
  QStringList bindings;
  
  if (cmd_table->check(bindings) && tool_table->check(bindings)) {
    Shortcut::raz();
    cmd_table->accept();
    tool_table->accept();
    Shortcut::save();
    QTabDialog::accept();
  }
}

//
// ShortcutTable
//

// copy/cut/paste

ShortcutTable::ShortcutTable(QWidget * parent, bool tool, int n)
    : MyTable(n + 1, 6, parent), for_tool(tool) {
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, TR("Shift"));
#ifdef __APPLE__
#include "../xpm/pomme_xpm.xpm"
  QPixmap pomme_xpm((const char **) pomme);
  QIconSet ic(pomme_xpm);
  horizontalHeader()->setLabel(1, ic, "");
#else
  horizontalHeader()->setLabel(1, TR("Ctrl"));
#endif
  horizontalHeader()->setLabel(2, TR("Alt"));
  horizontalHeader()->setLabel(3, TR("Key"));
  horizontalHeader()->setLabel(4, (tool) ? TR("tool display") : TR("command"));
  horizontalHeader()->setLabel(5, TR("do"));

  QStringList tools = Tool::all_display();

  values.append("");
  keys.append("");
  if (tool)
    values += tools;
  else {
    const QStringList & cmds = Shortcut::cmds();
    
    for (QStringList::ConstIterator it = cmds.begin(); it != cmds.end(); ++it)
      values.append(TR(*it));
  }
  keys += Shortcut::keys();
  
  const QMap<QString, QString> & shortcuts = Shortcut::shortcuts();
  QMap<QString, QString>::ConstIterator iter;
  int row = 0;

  for (iter = shortcuts.begin(); iter != shortcuts.end(); ++iter) {
    int tool_index = tools.findIndex(iter.data());
    
    if ((tool) ? (tool_index != -1) : (tool_index == -1)) {
      BooL shift;
      BooL ctrl;
      BooL alt;
      QString key = Shortcut::extract(iter.key(), shift, ctrl, alt);

      if (shift) setText(row, 0, TR("yes"));
      if (ctrl) setText(row, 1, TR("yes"));
      if (alt) setText(row, 2, TR("yes"));
      setItem(row, 3, new ComboItem(this, key, keys, FALSE));
      setItem(row, 4, new ComboItem(this, (tool) ? iter.data() : TR(iter.data()), values, FALSE));
      row += 1;
    }
  }
  sortColumn(3, TRUE, TRUE);
  
  setItem(row, 3, new ComboItem(this, QString::null, Shortcut::keys(), FALSE));
  setItem(row, 4, new ComboItem(this, QString::null, values, FALSE));

  adjustColumn(0);
  adjustColumn(1);
  adjustColumn(2);

  setColumnStretchable (4, TRUE);
  
  adjustColumn(5);
  setColumnStretchable(5, FALSE);
    
  connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	  this, SLOT(button_pressed(int, int, int, const QPoint &)));
  connect(this, SIGNAL(valueChanged(int, int)),
	  this, SLOT(value_changed(int, int)));
}

void ShortcutTable::value_changed(int row, int col) {
  if ((row == (numRows() - 1)) && (col == 3) && !text(row, 3).isEmpty())
    insert_row_after(row);
}

void ShortcutTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col < 3) {
    setText(row, col, (text(row, col).isEmpty()) ? TR("yes") : QString());
  }
  if (col == 5) {
    char s[16];
    
    sprintf(s, "%d", row + 1);
    
    QPopupMenu m;
    m.insertItem(TR("shortcut ") + s, -1);
    m.insertSeparator();
    m.insertItem(TR("Insert shortcut before"), 0);
    m.insertItem(TR("Insert shortcut after"), 1);
    m.insertSeparator();
    m.insertItem(TR("Delete shortcut"), 2);
    m.insertSeparator();
    m.insertItem(TR("Copy shortcut"), 3);
    m.insertItem(TR("Cut shortcut"), 4);
    m.insertItem(TR("Paste shortcut"), 5);
    m.insertSeparator();

    switch (m.exec(QCursor::pos())) {
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
      break;
    }
  }
}

void ShortcutTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    QTableItem * it;
    
    setText(index, 0, text(index - 1, 0));
    setText(index, 1, text(index - 1, 1));
    setText(index, 2, text(index - 1, 2));
    it = item(index - 1, 3);
    takeItem(it);
    setItem(index, 3, it);
    it = item(index - 1, 4);
    takeItem(it);
    setItem(index, 4, it);
  }
  
  setText(row, 0, QString::null);
  setText(row, 1, QString::null);
  setText(row, 2, QString::null);
  setItem(row, 3, new ComboItem(this, QString::null, Shortcut::keys(), FALSE));
  setItem(row, 4, new ComboItem(this, QString::null, values, FALSE));
}

void ShortcutTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    QTableItem * it;
    
    setText(index, 0, text(index - 1, 0));
    setText(index, 1, text(index - 1, 1));
    setText(index, 2, text(index - 1, 2));
    it = item(index - 1, 3);
    takeItem(it);
    setItem(index, 3, it);
    it = item(index - 1, 4);
    takeItem(it);
    setItem(index, 4, it);
  }
  
  setText(row + 1, 0, QString::null);
  setText(row + 1, 1, QString::null);
  setText(row + 1, 2, QString::null);
  setItem(row + 1, 3, new ComboItem(this, QString::null, Shortcut::keys(), FALSE));
  setItem(row + 1, 4, new ComboItem(this, QString::null, values, FALSE));
}

void ShortcutTable::delete_row(int row) {
  int n = numRows();
  int index;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    setText(row, 0, QString::null);
    setText(row, 1, QString::null);
    setText(row, 2, QString::null);
    setItem(row, 3, new ComboItem(this, QString::null, Shortcut::keys(), FALSE));
    setItem(row, 4, new ComboItem(this, QString::null, values, FALSE));
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      QTableItem * it;
      
      setText(index, 0, text(index + 1, 0));
      setText(index, 1, text(index + 1, 1));
      setText(index, 2, text(index + 1, 2));
      it = item(index + 1, 3);
      takeItem(it);
      setItem(index, 3, it);
      it = item(index + 1, 4);
      takeItem(it);
      setItem(index, 4, it);
    }
    setNumRows(n - 1);
  }
}

void ShortcutTable::copy_row(int row) {
  shift_copy = text(row, 0);
  control_copy = text(row, 1);
  alt_copy = text(row, 2);
  key_copy = text(row, 3);
  val_copy = text(row, 4);
}

void ShortcutTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void ShortcutTable::paste_row(int row) {
  setText(row, 0, shift_copy);
  setText(row, 1, control_copy);
  setText(row, 2, alt_copy);
  setText(row, 3, key_copy);
  setText(row, 4, val_copy);
  
  if ((row == (numRows() - 1)) &&
      (!key_copy.isEmpty() ||
       !val_copy.isEmpty()))
    insert_row_after(row);
}

bool ShortcutTable::check(QStringList & bindings) {
  forceUpdateCells();
  
  int n = numRows();
  int row;
      
  for (row = 0; row != n; row += 1) {
    if (!text(row, 3).isEmpty() && !text(row, 4).isEmpty()) {
      QString s;
      
      if (!text(row, 0).isEmpty())
	s = TR("Shift ");
      if (!text(row, 1).isEmpty())
#ifdef __APPLE__
	s += "Apple ";
#else
	s += TR("Ctrl ");
#endif
      if (!text(row, 2).isEmpty())
	s += TR("Alt ");
      s += text(row, 3);
      
      if (bindings.findIndex(s) == -1)
	bindings.append(s);
      else {
	msg_warning(TR("Shortcut"), TR("Several use of the shortcut '%1'", s));
	return FALSE;
      }
    }
  }
  
  return TRUE;
}
  
void ShortcutTable::accept() {
  const QStringList & cmds = Shortcut::cmds();
  int n = numRows();
  int row;
      
  for (row = 0; row != n; row += 1)
    if (!text(row, 3).isEmpty() && !text(row, 4).isEmpty())
      Shortcut::add(text(row, 3), !text(row, 0).isEmpty(),
		    !text(row, 1).isEmpty(), !text(row, 2).isEmpty(),
		    (for_tool) ? text(row, 4) : cmds[values.findIndex(text(row, 4)) - 1]);
}
