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
#include <qhbox.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qcursor.h>

#include "CodChangeMsgDialog.h"
#include "ColMsg.h"
#include "CodMsgSupport.h"
#include "CodObjCanvas.h"
#include "BrowserClass.h"
#include "OperationData.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "BrowserView.h"
#include "translate.h"

QSize CodChangeMsgDialog::previous_size;

CodChangeMsgDialog::CodChangeMsgDialog(QWidget * parent, ColMsg * m)
    : QDialog(parent, "Communication message dialog", TRUE), msg(m) {
  setCaption(TR("Communicationg message dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  
  SmallPushButton * b = new SmallPushButton(TR("message :"), this);
  
  hbox->addWidget(b);
  connect(b, SIGNAL(clicked()), this, SLOT(menu_op()));

  edoper = new QComboBox(TRUE, this);
  edoper->setAutoCompletion(completion());
  if (msg->operation == 0)
    edoper->insertItem(msg->explicit_operation);
  else
    edoper->insertItem(msg->operation->definition(TRUE, FALSE));
  
  CodObjCanvas * from;
  CodObjCanvas * to;
    
  msg->in->get_from_to(from, to, msg->is_forward);
  
  // gets operations
  cl = to->get_class();
  if (cl != 0) {
    cl->get_opers(opers, list);
    edoper->insertStringList(list);
    
    if (!cl->is_writable())
      cl = 0;
  }
  edoper->setCurrentItem(0);
    
  QSizePolicy sp = edoper->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  edoper->setSizePolicy(sp);
  
  hbox->addWidget(edoper);
  
  // ok & cancel
  
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
}

void CodChangeMsgDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

CodChangeMsgDialog::~CodChangeMsgDialog() {
  previous_size = size();
}

void CodChangeMsgDialog::menu_op() {
  QPopupMenu m(0);

  m.insertItem(TR("Choose"), -1);
  m.insertSeparator();
  
  int index = list.findIndex(edoper->currentText().stripWhiteSpace());
  
  if (index != -1)
    m.insertItem(TR("Select in browser"), 0);
  
  BrowserNode * bn = BrowserView::selected_item();
  
  if ((bn != 0) && 
      (bn->get_type() == UmlOperation) &&
      !bn->deletedp() &&
      (opers.findIndex((OperationData *) bn->get_data()) != -1))
    m.insertItem(TR("Choose operation selected in browser"), 1);
  else
    bn = 0;
  
  if (cl != 0)
    m.insertItem(TR("Create operation and choose it"), 2);
  
  if ((index != -1) || (bn != 0) || (cl != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      opers[index]->get_browser_node()->select_in_browser();
      break;
    case 2:
      bn = cl->add_operation();
      if (bn == 0)
	return;
      bn->select_in_browser();
      // no break
    case 1:
      {
	OperationData * od = (OperationData *) bn->get_data();

	if ((index = opers.findIndex(od)) == -1) {
	  index = opers.count();
	  opers.append(od);
	  
	  QString s = od->definition(TRUE, FALSE);
	  
	  list.append(s);
	  edoper->insertItem(s);
	}
      }
      edoper->setCurrentItem(index + 1);
    }
  }
}

void CodChangeMsgDialog::accept() {
  QString s = edoper->currentText().stripWhiteSpace();
  
  if (s.isEmpty())
    return;
  
  int index = list.findIndex(s);
  
  if (index >= 0) {
    msg->operation = opers[index];
    msg->explicit_operation = QString::null;
  }
  else {
    msg->operation = 0;
    msg->explicit_operation = s;
  }
  
  QDialog::accept();
}
