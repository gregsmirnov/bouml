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
#include <qgrid.h>
#include <qpopupmenu.h>
#include <qcursor.h>

#include "CodAddMsgDialog.h"
#include "ColDiagramView.h"
#include "CodMsgSupport.h"
#include "CodObjCanvas.h"
#include "ColMsg.h"
#include "BrowserClass.h"
#include "BrowserOperation.h"
#include "OperationData.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "BrowserView.h"
#include "translate.h"

QSize CodAddMsgDialog::previous_size;

CodAddMsgDialog::CodAddMsgDialog(CodObjCanvas * from, CodObjCanvas * to,
				 CodMsgSupport * i, ColDiagramView * v,
				 bool fo)
    : QDialog(0, "add msg dialog", TRUE), in(i), view(v), forward(fo) {
  setCaption(TR("Add message dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(10);
  QLabel * label1 = new QLabel(TR("Add message to %1", to->get_full_name()), this);
  label1->setAlignment(AlignCenter);
  hbox->addWidget(label1);
  
  QGrid * grid = new QGrid(2, this);
  
  vbox->addWidget(grid);
  new QLabel(TR("rank : "), grid);
  cbrank = new QComboBox(FALSE, grid);
  
  ColMsgList all_in;
  ColMsgList all_out;
  
  from->get_all_in_all_out(all_in, all_out);
  
  ColMsg * m;
  QStringList new_ones;
  QListIterator<ColMsg> itout(all_out);
  
  for (; (m = itout.current()) != 0; ++itout) {
    QString s = m->next_hierarchical_rank();
      
    if ((s.find('.') != - 1) && (ColMsg::find(s, all_out) == 0)) {
      cbrank->insertItem(QString::number(m->get_rank() + 1) + " : " + s);
      new_ones.append(s);
    }
  }
    
  QListIterator<ColMsg> itin(all_in);
  
  for (; (m = itin.current()) != 0; ++itin) {
    QString s = m->get_hierarchical_rank() + ".1";
    
    if ((ColMsg::find(s, all_out) == 0) && (new_ones.findIndex(s) == -1)) {
      cbrank->insertItem(QString::number(m->get_rank() + 1) + " : " + s);
      new_ones.append(s);
    }
  }

  // add a the rank for a new toplevel link
  cbrank->insertItem(QString::number(ColMsg::last_rank(view->get_msgs()) + 1)
		     + " : "
		     + QString::number(view->get_msgs().count() + 1));
  
  // the default new one follow the last input or output or view's last msg
  cbrank->setCurrentItem((cbrank->count() == 1) ? 0 : cbrank->count() - 2);
  
  QSizePolicy sp = cbrank->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  cbrank->setSizePolicy(sp);
  
  new QLabel("", grid);
  new QLabel("", grid);
  
  // the operations
  
  SmallPushButton * b = new SmallPushButton(TR("message :"), grid);
  
  connect(b, SIGNAL(clicked()), this, SLOT(menu_op()));
  
  edoper = new QComboBox(TRUE, grid);
  edoper->setAutoCompletion(completion());
  
  // gets operations
  cl = to->get_class();
  if (cl != 0) {
    cl->get_opers(opers, list);
    edoper->insertStringList(list);
    
    if (!cl->is_writable())
      cl = 0;
  }
    
  edoper->setSizePolicy(sp);
  
  new QLabel("", grid);
  new QLabel("", grid);
  
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

void CodAddMsgDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

CodAddMsgDialog::~CodAddMsgDialog() {
  previous_size = size();
}

void CodAddMsgDialog::menu_op() {
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

void CodAddMsgDialog::accept() {
  QString s = edoper->currentText().stripWhiteSpace();
  
  if (s.isEmpty())
    return;
  
  int index = list.findIndex(s);
  const OperationData * d;
  QString e;
  
  if (index >= 0)
    d = opers[index];
  else {
    d = 0;
    e = s;
  }

  s = cbrank->currentText();
  
  ColMsg::new_one(d, e, forward, s.mid(s.find(':') + 2), in)
    ->place_in(view->get_msgs());
  
  view->update_msg_supports();
  
  QDialog::accept();
}
