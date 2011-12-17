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





#include <qlabel.h>
#include <qlayout.h>
#include <qcombobox.h> 
#include <qpushbutton.h>
#include <qapplication.h>

#include "ReferenceDialog.h"
#include "BrowserView.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "translate.h"

ReferenceDialog * ReferenceDialog::the;
QSize ReferenceDialog::previous_size;

ReferenceDialog::ReferenceDialog(BrowserNode * bn)
    : QDialog(0, "Referenced By dialog", FALSE, WDestructiveClose) {
  the = this;
  target = bn;
  
  setCaption(TR("Referenced By dialog"));
    
  QVBoxLayout * vbox = new QVBoxLayout(this);
 
  vbox->setMargin(5);
 
  QString s = target->get_name();
  
  s += TR(" is referenced by :");
  
  vbox->addWidget(new QLabel(s, this));
  
  results = new QComboBox(FALSE, this);
  vbox->addWidget(results);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
  QPushButton * search_b = new QPushButton(TR("Recompute"), this);
  QPushButton * close_b = new QPushButton(TR("Close"), this);
  
  hbox->setMargin(5);
  hbox->addWidget(search_b);
  hbox->addWidget(select_b = new QPushButton(TR("Select"), this));
  hbox->addWidget(mark_unmark_b = new QPushButton(TR("Unmark"), this));
  hbox->addWidget(mark_them_b = new QPushButton(TR("Mark them"), this));
  hbox->addWidget(unmark_all_b = new QPushButton(TR("Unmark all"), this));
  hbox->addWidget(close_b);
    
  search_b->setDefault(TRUE);
  
  connect(search_b, SIGNAL(clicked()), this, SLOT(compute()));
  connect(select_b, SIGNAL(clicked()), this, SLOT(select()));
  connect(close_b, SIGNAL(clicked()), this, SLOT(reject()));
  connect(mark_unmark_b, SIGNAL(clicked()), this, SLOT(mark_unmark()));
  connect(mark_them_b, SIGNAL(clicked()), this, SLOT(mark_them()));
  connect(unmark_all_b, SIGNAL(clicked()), this, SLOT(unmark_all()));
  connect(results, SIGNAL(activated(int)), this, SLOT(selected(int)));

  compute();
  
  open_dialog(this);
}

ReferenceDialog::~ReferenceDialog() {
  the = 0;
  previous_size = size();
  
  close_dialog(this);
}

void ReferenceDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void ReferenceDialog::compute() {
  QApplication::setOverrideCursor(Qt::waitCursor);

  QList<BrowserNode> l;
  BrowserNode * bn;
  
  nodes.clear();
  results->clear();
  target->referenced_by(l);
  for (bn = l.first(); bn; bn = l.next())
    nodes.append(bn);
  
  nodes.sort();
  
  // remove duplicats
  nodes.first();
  while ((bn = nodes.current()) != 0)
    if (bn == nodes.next())
      nodes.remove();
  
  QStringList names;
  
  nodes.full_names(names);
  
  QStringList::Iterator it;
  
  for (bn = nodes.first(), it = names.begin();
       bn;
       bn = nodes.next(), ++it)
    results->insertItem(*(bn->pixmap(0)), *it);
  
  selected((nodes.isEmpty()) ? -1 : 0);

  QApplication::restoreOverrideCursor();
}

void ReferenceDialog::selected(int index) {
  if (index == -1) {
    select_b->setEnabled(FALSE);
    mark_unmark_b->setEnabled(FALSE);
    mark_them_b->setEnabled(FALSE);
  }
  else {
    select_b->setEnabled(TRUE);
    mark_unmark_b->setEnabled(TRUE);
    mark_unmark_b->setText((nodes.at(index)->markedp())
			   ? TR("Unmark") : TR("Mark"));
    mark_them_b->setEnabled(TRUE);
  }
  
  unmark_all_b->setEnabled(!BrowserNode::marked_nodes().isEmpty());
}

void ReferenceDialog::mark_unmark() {
  BrowserNode * bn = nodes.at(results->currentItem());
  
  bn->toggle_mark();  	// call update
  BrowserView::force_visible(bn);
}

void ReferenceDialog::mark_them() {
  BrowserNode * bn;
    
  for (bn = nodes.first(); bn != 0; bn = nodes.next()) {
    if (! bn->markedp()) {
      bn->toggle_mark();  	// call update
      BrowserView::force_visible(bn);
    }
  }
}

void ReferenceDialog::unmark_all() {
  BrowserNode::unmark_all();  	// call update
}

void ReferenceDialog::select() {
  if (!nodes.isEmpty())
    nodes.at(results->currentItem())->select_in_browser();
}

void ReferenceDialog::update() {
  selected((results->count() != 0) ? results->currentItem() : -1);
}

void ReferenceDialog::show(BrowserNode * target)
{
  if (the != 0)
    the->close(TRUE);
  
  (new ReferenceDialog(target))->QDialog::show();
}
