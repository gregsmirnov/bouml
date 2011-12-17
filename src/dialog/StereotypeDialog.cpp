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

#include "StereotypeDialog.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "translate.h"

QSize StereotypeDialog::previous_size;

StereotypeDialog::StereotypeDialog(const QStringList &defaults,
				   QString & st, QString & la,
				   QString cap, QString lbl)
    : QDialog(0, cap, TRUE), ste(st), lab(la) {
  setCaption(cap);
  
  if (! ste.isEmpty())
    // removes << and >>
    ste = ste.mid(2, ste.length() - 4);
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  QLabel * label1;
  QLabel * label2;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(label1 = new QLabel(lbl, this));
  ed = new LineEdit(la, this);
  hbox->addWidget(ed);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(label2 = new QLabel(TR("stereotype : "), this));
  cb = new QComboBox(TRUE, this);
  cb->insertItem(ste);
  cb->setCurrentItem(0);
  cb->insertStringList(defaults);
  cb->setAutoCompletion(completion());
  hbox->addWidget(cb);
  
  QSizePolicy sp = cb->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  cb->setSizePolicy(sp);
  
  same_width(label1, label2);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * accept = new QPushButton(TR("&OK"), this);
  QPushButton * cancel = new QPushButton(TR("&Cancel"), this);
  QSize bs(cancel->sizeHint());
  
  accept->setDefault(TRUE);
  accept->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  hbox->addWidget(accept);
  hbox->addWidget(cancel);
    
  connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

StereotypeDialog::~StereotypeDialog() {
  previous_size = size();
}

void StereotypeDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

void StereotypeDialog::accept() {
  ste = cb->currentText().stripWhiteSpace();

  if (!ste.isEmpty())
    ste = QString("<<") + ste + ">>";
  
  lab = ed->text();
  
  QDialog::accept();
}
