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

#error not used anymore





#include <qcursor.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>
#include <qtextcodec.h>
#include <qstringlist.h>

#include "CharSetDialog.h"
#include "UmlDesktop.h"
#include "translate.h"

QSize CharSetDialog::previous_size;

CharSetDialog::CharSetDialog(QString cs) : QDialog(0, "charset dialog", TRUE) {
  setCaption(TR("Character set dialog"));
  move(QCursor::pos());
 
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBoxLayout * hbox;
 
  vbox->setMargin(5);
  
  QLabel * lbl = new QLabel(TR("ERROR : No codec for '%1'\n", cs), this);
  
  lbl->setAlignment(::Qt::AlignCenter);
  vbox->addWidget(lbl);
  
  vbox->addWidget(new QLabel(TR("\nCharset MUST be changed\n\n"
				"For this session, choose one of the following else 'ISO_8859-1' will be used"),
			     this));
  
  cb = new QComboBox(FALSE, this);
  cb->setAutoCompletion(completion());
  vbox->addWidget(cb);
  
  QStringList l;
  QTextCodec * co;
  int i = 0;
  
  while ((co = QTextCodec::codecForIndex(i++)) != 0) {
    QString na = co->name();
    int pos = 0;  
    
    while ((pos = na.find(' ', pos)) != -1)
      na.replace(pos, 1, "_");
    
    if (QTextCodec::codecForName(na) == co)
      l.append(na);
  }
  
  l.sort();
  cb->insertStringList(l);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * ok = new QPushButton(TR("&OK"), this);
  QPushButton * cancel = new QPushButton(TR("&Cancel"), this);
  QSize bs(cancel->sizeHint());
  
  ok->setDefault(TRUE);
  ok->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  hbox->addWidget(ok);
  hbox->addWidget(cancel);
  
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
  
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

CharSetDialog::~CharSetDialog() {
  previous_size = size();
}

QString CharSetDialog::choosen() {
  return cb->currentText();
}
