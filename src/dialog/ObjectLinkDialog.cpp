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
#include <qpushbutton.h>

#include "ObjectLinkDialog.h"
#include "MyTable.h"
#include "BrowserClass.h"
#include "BrowserClassInstance.h"
#include "ClassInstanceData.h"
#include "BrowserRelation.h"
#include "RelationData.h"
#include "UmlDesktop.h"
#include "translate.h"

QSize ObjectLinkDialog::previous_size;

ObjectLinkDialog::ObjectLinkDialog(BrowserClassInstance * a, BrowserClassInstance * b,
				   QList<RelationData> & l, RelationData * current,
				   int nfirstdir)
    : QDialog(0, "object link dialog", TRUE),
      rels(l), nforward(nfirstdir), clia(a), clib(b), choozen(0), reverse(FALSE) {
  setCaption(TR("Object link dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);
  QHBoxLayout * hbox;
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  
  hbox->addWidget(new QLabel(TR("\n"
				"To set the association, select a cell or the first column with a single click, then press 'OK'\n"
				"To unset the association press 'Unset' then press 'OK'\n"),
			     this));
  
  init(current);	// update table
  
  vbox->addWidget(table);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * newrel = 
    (((ClassInstanceData *) clia->get_data())->get_class()->is_writable() ||
     ((ClassInstanceData *) clib->get_data())->get_class()->is_writable())
    ? new QPushButton(TR("&New"), this) : 0;
  QPushButton * unset = new QPushButton(TR("&Unset"), this);
  QPushButton * accept = new QPushButton(TR("&OK"), this);
  QPushButton * cancel = new QPushButton(TR("&Cancel"), this);
  QSize bs(cancel->sizeHint());
  
  if (newrel != 0) {
    newrel->setFixedSize(bs);
    hbox->addWidget(newrel);
    connect(newrel, SIGNAL(clicked()), this, SLOT(create()));
  }
  unset->setFixedSize(bs);
  accept->setFixedSize(bs);
  cancel->setFixedSize(bs);  
    
  hbox->addWidget(unset);
  hbox->addWidget(accept);
  hbox->addWidget(cancel);
  
  connect(unset, SIGNAL(clicked()), this, SLOT(unselect()));
  connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

ObjectLinkDialog::~ObjectLinkDialog() {
  previous_size = size();
}

void ObjectLinkDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

static void add_rel(MyTable * table, RelationData * d, int row,
		    QString a, QString b)
{
  table->setItem(row, 0, new TableItem(table, QTableItem::Never, a));

  const char * s;
  
  s = d->get_role_b();
  table->setItem(row, 1, new TableItem(table, QTableItem::Never, (s == 0) ? "" : s));
  
  table->setItem(row, 2, new TableItem(table, QTableItem::Never, stringify(d->get_type())));
  
  s = d->get_role_a();
  table->setItem(row, 3, new TableItem(table, QTableItem::Never, (s == 0) ? "" : s));
  
  table->setItem(row, 4, new TableItem(table, QTableItem::Never, b));
}

void ObjectLinkDialog::init(RelationData * current) {
  table = new MyTable(this);
  
  table->setNumRows(rels.count());
  table->setNumCols(5);
  table->setSelectionMode(QTable::Single);
  table->setSorting(FALSE);
  
  table->horizontalHeader()->setLabel(0, TR("Class Inst."));
  table->horizontalHeader()->setLabel(1, TR("Role"));
  table->horizontalHeader()->setLabel(2, TR("kind"));
  table->horizontalHeader()->setLabel(3, TR("Role"));
  table->horizontalHeader()->setLabel(4, TR("Class Inst."));
  
  ra = clia->get_name() + QString(":") +
    ((ClassInstanceData *) clia->get_data())->get_class()->get_name();
  rb = clib->get_name() + QString(":") +
    ((ClassInstanceData *) clib->get_data())->get_class()->get_name();
  QListIterator<RelationData> iter(rels);
  int row;
  
  for (row = 0; (row != nforward) && iter.current(); ++iter, row += 1)
    add_rel(table, iter.current(), row, ra, rb);
    
  for (; iter.current(); ++iter, row += 1)
    add_rel(table, iter.current(), row, rb, ra);
    
  ninputrels = row;
  
  table->setColumnStretchable (1, TRUE);
  table->setColumnStretchable (3, TRUE);
  table->adjustColumn(0);
  table->adjustColumn(2);
  table->adjustColumn(4);
  
  if (current != 0) {
    // select the current relation
    QTableSelection sel;
    int row = rels.findRef(current);
    
    sel.init(row, 0);
    sel.expandTo(row, 4);
    table->addSelection(sel);
  }
}

void ObjectLinkDialog::unselect() {
  // multi selection possible even table->setSelectionMode(QTable::Single)
  while (table->numSelections() != 0)
    table->removeSelection(0);
}

void ObjectLinkDialog::create() {
  BrowserClass * cla = ((ClassInstanceData *) clia->get_data())->get_class();
  BrowserClass * clb = ((ClassInstanceData *) clib->get_data())->get_class();
  RelationData * d = (cla->is_writable())
    ? (RelationData *) cla->add_relation((clb->is_writable()) ? UmlAssociation
							      : UmlDirectionalAssociation,
					 clb)
    : (RelationData *) clb->add_relation(UmlDirectionalAssociation, cla);

  d->get_start()->select_in_browser();
  d->edit();
  
  int n = table->numRows();
  
  table->setNumRows(n + 1);
  add_rel(table, d, n, ra, rb);
  rels.append(d);
  
  // multi selection possible even table->setSelectionMode(QTable::Single)
  while (table->numSelections() != 0)
    table->removeSelection(0);
  
  QTableSelection sel;
  
  sel.init(n, 0);
  sel.expandTo(n, 4);
  table->addSelection(sel);
}

void ObjectLinkDialog::accept() {
  if (table->numSelections() != 0) {
    QTableSelection sel = table->selection(0);
    
    choozen = rels.at(sel.topRow());
    reverse = (sel.topRow() >= nforward) && (sel.topRow() < ninputrels);
  }
  QDialog::accept();
}
