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





#include <qcursor.h>
#include <qlayout.h>
#include <qcombobox.h> 
#include <qpushbutton.h>

#include "OperationListDialog.h"
#include "BrowserOperation.h"
#include "OperationData.h"
#include "UmlDesktop.h"
#include "translate.h"

QSize OperationListDialog::previous_size;

OperationListDialog::OperationListDialog(const char * m,
					 QList<BrowserOperation> & l)
    : QDialog(0, m, TRUE) {
  setCaption(m);
  move(QCursor::pos());
 
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBoxLayout * hbox;
 
  vbox->setMargin(5);
 
  cb = new QComboBox(FALSE, this);
  vbox->addWidget(cb);
  
  for (BrowserOperation * oper = l.first(); oper; oper = l.next()) {
    QString s = ((BrowserNode *) oper->parent())->get_name() +
      QString("::") + oper->get_data()->definition(TRUE, FALSE);
    
    if (((OperationData *) oper->get_data())->get_is_abstract())
      cb->insertItem("[a] " + s);
    else
      cb->insertItem(s);
  }
  
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

  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
  
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

OperationListDialog::~OperationListDialog() {
  previous_size = size();
}

int OperationListDialog::choosen() {
  return cb->currentItem();
}
