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





#include <qwidget.h>

#include "MLinesItem.h"
#if QT_VERSION == 230
#include "MLEDialog.h"
#include "strutil.h"
#else
#include <qmultilineedit.h>
#include "DialogUtil.h"
#endif

MLinesItem::MLinesItem(QTable * ta, const QString & s)
    : TableItem(ta, QTableItem::WhenCurrent, s) {
}

QWidget * MLinesItem::createEditor() const {
#if QT_VERSION == 230
  ((MLinesItem *) this)->mle = new MLEDialog(fromUnicode(text()), FALSE);
#else
  ((MLinesItem *) this)->mle = new QMultiLineEdit(table()->viewport());
  mle->setText(text());
#endif
  return mle;
}

void MLinesItem::setContentFromEditor(QWidget * w) {
#if QT_VERSION == 230
  if (w->inherits("MLEDialog")) {
    QString s = toUnicode(((MLEDialog *) w)->text());
    
    setText(s);
  }
#else
  if (w->inherits("QMultiLineEdit")) {
    QString s = ((QMultiLineEdit *) w)->text();
    
    setText(s);
  }
#endif
  else
    QTableItem::setContentFromEditor(w);
}

