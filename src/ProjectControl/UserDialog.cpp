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
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>
#include <qlineedit.h> 

#include "UserDialog.h"
#include "myio.h"

UserDialog::UserDialog(const QPoint & p)
    : QDialog(0, "Browser search", TRUE) {
  setCaption("Choose user");
  move(p);
  
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBoxLayout * hbox;
  QLabel * lbl1;
  QLabel * lbl2;
  
  vbox->setMargin(5);

  QLabel * lbl = 
    new QLabel("\nSelect a user to get its identity,\nyou may change or set his name\n", 
	       this);
  
  lbl->setAlignment(::Qt::AlignCenter);
  vbox->addWidget(lbl);
  
  hbox = new QHBoxLayout(vbox);
  hbox->setMargin(5);
  lbl1 = new QLabel("User : ", this);
  hbox->addWidget(lbl1);
  ids = new QComboBox(FALSE, this);
  for (int index = 2; index != 128; index += 1) {
    QString s = user_name(index);
    
    ids->insertItem((s.isEmpty()) 
		    ? QString::number(index)
		    : QString::number(index) + " (" + s + ")");
  }
  ids->setCurrentItem(user_id() - 2);
  
  QSizePolicy sp = ids->sizePolicy();

  sp.setHorData(QSizePolicy::Expanding);
  ids->setSizePolicy(sp);

  hbox->addWidget(ids);
  
  hbox = new QHBoxLayout(vbox);
  hbox->setMargin(5);
  lbl2 = new QLabel("New name : ", this);
  lbl1->setFixedWidth(lbl2->sizeHint().width());
  hbox->addWidget(lbl2);
  edname = new QLineEdit(this);
  hbox->addWidget(edname);
    
  hbox = new QHBoxLayout(vbox);
  hbox->setMargin(5);

  QPushButton * ok = new QPushButton("Ok", this);
  QPushButton * cancel = new QPushButton("Cancel", this);
  
  hbox->addWidget(new QLabel(this));
  hbox->addWidget(ok);
  hbox->addWidget(new QLabel(this));
  hbox->addWidget(cancel);
  hbox->addWidget(new QLabel(this));
  
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

UserDialog::~UserDialog() {
}

int UserDialog::id() {
  return ids->currentItem() + 2;
}

QString UserDialog::name() {
  QString s = edname->text().stripWhiteSpace();
  
  return (s.isEmpty()) ? user_name(id()) : s;
}

