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
#include <qpopupmenu.h>
#include <qgrid.h>
#include <qcursor.h>

#include "InstanceDialog.h"
#include "Instance.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "BrowserView.h"
#include "translate.h"

QSize InstanceDialog::previous_size;

InstanceDialog::InstanceDialog(Instance * i, QString w, UmlCode k)
    : QDialog(0, w + " instance dialog", TRUE),
      inst(i), what(w), kind(k) {
  setCaption(TR(what + " instance dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  
  vbox->setMargin(5);
  
  QGrid * grid = new QGrid(2, this);
  
  vbox->addWidget(grid);
  new QLabel(TR("name : "), grid);
  edname = new LineEdit(inst->get_name(), grid);
  edname->setFocus();
  
  new QLabel("", grid);
  new QLabel("", grid);
  
  SmallPushButton * b =
    new SmallPushButton(TR(what) + " :", grid);
  
  connect(b, SIGNAL(clicked()), this, SLOT(menu_type()));
  
  edtype = new QComboBox(FALSE, grid);
  inst->get_types(nodes);
  nodes.full_names(list);
  edtype->insertStringList(list);
  edtype->setCurrentItem(nodes.find(inst->get_type()));
  
  new QLabel("", grid);
  new QLabel("", grid);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  
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
  
  open_dialog(this);
}

void InstanceDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

InstanceDialog::~InstanceDialog() {
  previous_size = size();
  close_dialog(this);
}

void InstanceDialog::menu_type() {
  QPopupMenu m(0);

  m.insertItem(TR("Choose"), -1);
  m.insertSeparator();
  
  int index = list.findIndex(edtype->currentText().stripWhiteSpace());
  
  if (index != -1)
    m.insertItem(TR("Select in browser"), 0);
  
  BrowserNode * bn = BrowserView::selected_item();
    
  if ((bn != 0) && 
      (bn->get_type() == kind) && !bn->deletedp())
    m.insertItem(TR("Choose " + what + " selected in browser"), 1);
  else
    bn = 0;
  
  bool new_available = inst->new_type_available();
  
  if (new_available)
    m.insertItem(TR("Create " + what + " and choose it"), 2);
  
  if (new_available || (index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      nodes.at(index)->select_in_browser();
      break;
    case 2:
      bn = inst->new_type();
      if (bn == 0)
	return;
      bn->select_in_browser();
      // no break
    case 1:

      {
	QString s = bn->full_name(TRUE);
	
	if ((index = list.findIndex(s)) == -1) {
	  // new class, may be created through an other dialog
	  index = 0;
	  QStringList::Iterator iter = list.begin();
	  QStringList::Iterator iter_end = list.end();
	  
	  while ((iter != iter_end) && (*iter < s)) {
	    ++iter;
	    index += 1;
	  }
	  nodes.insert((unsigned) index, bn);
	  list.insert(iter, s);
	  edtype->insertItem(s, index);
	}
      }
      edtype->setCurrentItem(index);
    }
  }
}

void InstanceDialog::accept() {
  inst->set_name(edname->text().stripWhiteSpace());
  inst->set_type(nodes.at(edtype->currentItem()));
  
  QDialog::accept();
}
