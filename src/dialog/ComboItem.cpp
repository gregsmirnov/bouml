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





#include <qcombobox.h> 

#include "ComboItem.h"
#include "DialogUtil.h"

ComboItem::ComboItem(QTable * ta, const QString & s,
		     const QStringList & list, bool edit)
    : TableItem(ta, QTableItem::WhenCurrent, s), editable(edit), cb(0), types(list) {
  if (editable)
    setReplaceable(FALSE);	// sinon le combo n est pas editable (!)
}

QWidget * ComboItem::createEditor() const {
  ((ComboItem *) this)->cb = new QComboBox(table()->viewport());
  // setEditable doit etre fait maintenant sinon les items sont perdus !
  cb->setEditable(editable);
  cb->setAutoCompletion(completion());
  cb->insertItem(text());
  cb->insertStringList(types);

  int index = types.findIndex(text());
  
  if (index != -1)
    cb->setCurrentItem(index + 1);
  
  return cb;
}

void ComboItem::setContentFromEditor(QWidget *w) {
  if (w->inherits("QComboBox"))
    setText(((QComboBox *) w)->currentText());
  else
    QTableItem::setContentFromEditor(w);
}

