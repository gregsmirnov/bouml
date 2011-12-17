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

#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qhbox.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h> 
#include <qpopupmenu.h>
#include <qcursor.h>

#include "SdMsgDialog.h"
#include "SdMsgBaseCanvas.h"
#include "BrowserClass.h"
#include "OperationData.h"
#include "SdDurationCanvas.h"
#include "SdLifeLineCanvas.h"
#include "SdObjCanvas.h"
#include "LabelCanvas.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "BrowserView.h"
#include "translate.h"

QSize SdMsgDialog::previous_size;

SdMsgDialog::SdMsgDialog(SdMsgBaseCanvas * a, const QStringList & defaults)
    : QDialog(0, "Message editor", TRUE), mc(a) {
  setCaption(TR("Message dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  
  SmallPushButton * b = new SmallPushButton(TR("message :"), this);
  
  hbox->addWidget(b);
  connect(b, SIGNAL(clicked()), this, SLOT(menu_op()));
  
  edoper = new QComboBox(TRUE, this);
  if (a->msg == 0)
    edoper->insertItem(a->explicit_msg);
  else
    edoper->insertItem(a->msg->definition(TRUE, FALSE));
  edoper->setAutoCompletion(completion());

  // gets operations
  if (!a->get_dest()->isaDuration())
    cl = 0;
  else if ((cl = ((SdDurationCanvas *) a->get_dest())->get_line()->get_obj()->get_class())
	   != 0) {
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
  
  //
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(new QLabel(TR("stereotype : "), this));
  edst = new QComboBox(TRUE, this);
  if (a->stereotype != 0) {
    QString s = a->stereotype->get_name();
    
    s = s.mid(2, s.length() - 4);
    edst->insertItem(s);
  }
  else
    edst->insertItem("");
  edst->setCurrentItem(0);
  edst->insertStringList(defaults);
  edst->setAutoCompletion(completion());
  hbox->addWidget(edst);
  edst->setSizePolicy(sp);
  
  //
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  
  QHBox * htab = new QHBox(this);
  
  htab->setMargin(5);
  
  QButtonGroup * bg = new QButtonGroup(2, Qt::Horizontal, TR("Message type"), htab);
  
  bg->setExclusive(TRUE);
  synchronous_rb = new QRadioButton(TR("synchronous"), bg);
  asynchronous_rb = new QRadioButton(TR("asynchronous"), bg);
  
  if (a->is_synchronous())
    synchronous_rb->setChecked(TRUE);
  else
    asynchronous_rb->setChecked(TRUE);

  hbox->addWidget(htab);
  
  vbox->addWidget(new QLabel(TR("\n\nWhen the arguments are specified they replace the \
operation's parameter(s) without any check"),
			     this));
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  hbox->addWidget(new QLabel(TR("arguments : "), this));
  edargs = new MultiLineEdit(this);
  edargs->setText(a->get_args());
  hbox->addWidget(edargs);  
  edargs->setFocus();
  
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

void SdMsgDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

SdMsgDialog::~SdMsgDialog() {
  previous_size = size();
}

void SdMsgDialog::menu_op() {
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

void SdMsgDialog::accept() {
  QString s = edoper->currentText().stripWhiteSpace();
  
  if (!s.isEmpty()) {
    int index = list.findIndex(s);
    
    if (index >= 0)
      mc->set_msg(opers[index], QString::null, edargs->text().stripWhiteSpace());
    else
      mc->set_msg(0, s, edargs->text().stripWhiteSpace());
  }
  else
    mc->set_msg(0, QString::null, QString::null);
  
  mc->set_synchronous(synchronous_rb->isChecked());
  
  s = edst->currentText().stripWhiteSpace();
  if (!s.isEmpty())
    s = "<<" + s + ">>";
  mc->update_st(s);

  QDialog::accept();
}
