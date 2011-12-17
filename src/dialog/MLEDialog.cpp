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





#include <qlayout.h> 
#include <qhbox.h>
#include <qlabel.h>

#include "MLEDialog.h"
#include "DialogUtil.h"
#include "translate.h"

MLEDialog::MLEDialog(QString t, bool buttons) : QDialog(0, "Uml", TRUE, 0) {
  QVBoxLayout *vbox = new QVBoxLayout(this);

  e = new MultiLineEdit(this);
  vbox->addWidget(e);
  
  QHBox * hbox = new QHBox(this);
  
  vbox->addWidget(hbox);
  hbox->setMargin(5);

  if (buttons) {
    new QLabel(hbox);
    ok = new QPushButton(TR("OK"), hbox);
    new QLabel(hbox);
    cancel = new QPushButton(TR("Cancel"), hbox);
    new QLabel(hbox);
  
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
  }
    
  e->setText(t);
}

MLEDialog::~MLEDialog() {
}

void MLEDialog::get(QString & t, const QPoint & p, QSize & sz)
{
  MLEDialog d(t, TRUE);
  
  d.move(p);
  if (sz.isValid())
    d.resize(sz);
  d.e->setFocus();
  if (d.exec() == QDialog::Accepted)
    t = d.e->text();
  sz = d.size();
}

QString MLEDialog::text() {
  return e->text();
}

