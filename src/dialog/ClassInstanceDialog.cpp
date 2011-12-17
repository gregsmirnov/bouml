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





#include <qgrid.h> 
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qpushbutton.h>
#include <qpopupmenu.h> 
#include <qcursor.h>

#include "ClassInstanceDialog.h"
#include "BrowserClassInstance.h"
#include "ClassInstanceData.h"
#include "RelationData.h"
#include "BrowserClass.h"
#include "BrowserAttribute.h"
#include "BrowserView.h"
#include "DialogUtil.h"
#include "BodyDialog.h"
#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "KeyValueTable.h"
#include "strutil.h"
#include "ProfiledStereotypes.h"
#include "translate.h"

RelTable::RelTable(QWidget * parent, ClassInstanceData * inst, bool visit)
    : MyTable(parent) {
  const QValueList<SlotRel> & rels = inst->get_relations();
  
  setNumRows(rels.count());
  setNumCols((visit) ? 5 : 6);
  setSelectionMode(QTable::Single);
  setSorting(FALSE);
  
  horizontalHeader()->setLabel(0, TR("Class Inst."));
  horizontalHeader()->setLabel(1, TR("Role"));
  horizontalHeader()->setLabel(2, TR("kind"));
  horizontalHeader()->setLabel(3, TR("Role"));
  horizontalHeader()->setLabel(4, TR("Class Inst."));
  horizontalHeader()->setLabel(5, TR("Delete"));
  
  QString role = inst->get_browser_node()->get_name() + QString(":") +
    inst->get_class()->get_name();
  int row = 0;
  QValueList<SlotRel>::ConstIterator it;
  
  for (it = rels.begin(); it != rels.end(); ++it)
    init_row(*it, row++, role, visit);

  setColumnStretchable (1, TRUE);
  setColumnStretchable (3, TRUE);
  adjustColumn(0);
  adjustColumn(2);
  adjustColumn(4);
  if (! visit)
    adjustColumn(5);
  
  connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	  this, SLOT(button_pressed(int, int, int, const QPoint &)));
}

void RelTable::init_row(const SlotRel & sr, int row, QString a, bool visit) {
  QString b = sr.value->get_name() + QString(":") +
    ((ClassInstanceData *) sr.value->get_data())->get_class()->get_name();
  
  setItem(row, 0, new TableItem(this, QTableItem::Never,
				      (sr.is_a) ? a : b));

  RelationData * d = sr.rel;
  const char * s;
  
  s = d->get_role_b();
  setItem(row, 1, new TableItem(this, QTableItem::Never, (s == 0) ? "" : s));
  
  setItem(row, 2, new TableItem(this, QTableItem::Never, stringify(d->get_type())));
  
  s = d->get_role_a();
  setItem(row, 3, new TableItem(this, QTableItem::Never, (s == 0) ? "" : s));
  
  setItem(row, 4, new TableItem(this, QTableItem::Never,
				      (sr.is_a) ? b : a));
  
  if (! visit)
    setText(row, 5, "");
}

void RelTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 5)
    setText(row, col, text(row, col).isEmpty() ? TR("   Yes") : QString());
}

//

QSize ClassInstanceDialog::previous_size;

ClassInstanceDialog::ClassInstanceDialog(ClassInstanceData * i)
    : QTabDialog(0, "class instance dialog", FALSE, WDestructiveClose),
      inst(i), atbl(0), rtbl(0) {
  setCaption(TR("Class instance dialog"));
  
  BrowserNode * bn = inst->get_browser_node();
  
  bn->edit_start();
  if (bn->is_writable()) {
    setOkButton(TR("OK"));
    setCancelButton(TR("Cancel"));
  }
  else {
    setOkButton(QString::null);
    setCancelButton(TR("Close"));
  }
  
  visit = !hasOkButton();
  
  QGrid * grid;
    
  // general tab
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("name : "), grid);
  edname = new LineEdit(bn->get_name(), grid);
  if (visit)
    edname->setReadOnly(TRUE);
  
  new QLabel(TR("stereotype :"), grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(bn->get_stereotype()));
  if (! visit) {
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlClassInstance));
    edstereotype->setAutoCompletion(completion());
  }
  SmallPushButton *  b = new SmallPushButton(TR("class :"), grid);
  
  connect(b, SIGNAL(clicked()), this, SLOT(menu_class()));

  edtype = new QComboBox(FALSE, grid);
  if (visit) {
    edtype->insertItem(inst->get_class()->full_name());
    nodes.append(inst->get_class());
  }
  else {
    BrowserClass::instances(nodes);
    nodes.full_names(list);
    edtype->insertStringList(list);
    edtype->setCurrentItem(nodes.find(inst->get_class()));
    connect(edtype, SIGNAL(activated(int)), this, SLOT(type_changed(int)));
  }
  if (visit)
    cl_container = 0;
  else {
    cl_container = (BrowserNode *) bn->parent();
    if ((cl_container != 0) && !cl_container->is_writable())
      cl_container = 0;
  }
  
  QVBox * vtab = new QVBox(grid);
  
  new QLabel(TR("description :"), vtab);
  if (! visit) {
    connect(new SmallPushButton(TR("Editor"), vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  }
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  
  QFont font = comment->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  // attributes tab
  
  atbl = new MyTable(this);
  atbl->setNumCols(3);
  atbl->setSorting(FALSE);
  atbl->setSelectionMode(QTable::NoSelection);	// single does not work
  atbl->setRowMovingEnabled(TRUE);
  atbl->horizontalHeader()->setLabel(0, TR(" Attribute "));
  atbl->horizontalHeader()->setLabel(1, TR(" Class "));
  atbl->horizontalHeader()->setLabel(2, TR(" Value "));
  
  addTab(atbl, TR("Attributes"));
  
  // relation tab
  
  if (! inst->relations.isEmpty()) {
    rtbl = new RelTable(this, inst, visit);
    addTab(rtbl, TR("Relations"));
  }
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(bn, grid, visit);
  addTab(grid, TR("Properties"));
  
  type_changed(edtype->currentItem());
  
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(update_all_tabs(QWidget *)));
  
  open_dialog(this);
}

void ClassInstanceDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

ClassInstanceDialog::~ClassInstanceDialog() {
  inst->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
  
  close_dialog(this);
}

void ClassInstanceDialog::update_all_tabs(QWidget * w) {
  if (w == atbl) {
    if (! visit)
      edname->setFocus();
  }
}

void ClassInstanceDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       inst, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ClassInstanceDialog::post_edit_description(ClassInstanceDialog * d, QString s)
{
  d->comment->setText(s);
}

void ClassInstanceDialog::menu_class() {
  QPopupMenu m(0);

  m.insertItem(TR("Choose"), -1);
  m.insertSeparator();
  
  int index = list.findIndex(edtype->currentText().stripWhiteSpace());
  
  if (index != -1)
    m.insertItem(TR("Select in browser"), 0);
  
  BrowserNode * bn = 0;
  
  if (! visit) {
    bn = BrowserView::selected_item();
    
    if ((bn != 0) && (bn->get_type() == UmlClass) && !bn->deletedp())
      m.insertItem(TR("Choose class selected in browser"), 1);
    else
      bn = 0;
    
    if (cl_container != 0)
      m.insertItem(TR("Create class and choose it"), 2);
  }
  
  if ((index != -1) || (bn != 0) || (cl_container != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      nodes.at(index)->select_in_browser();
      break;
    case 2:
      bn = BrowserClass::add_class(FALSE, cl_container);
      if (bn == 0)
	return;
      bn->select_in_browser();
      // no break
    case 1:
      {
	
	if ((index = nodes.findRef(bn)) == -1) {
	  // new class, may be created through an other dialog
	  QStringList::Iterator iter = list.begin();
	  QStringList::Iterator iter_end = list.end();
	  QString s = bn->full_name(TRUE);
	  
	  index = 0;
	  
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
      type_changed(index);
      break;
    default:
      break;
    }
  }
}

void ClassInstanceDialog::type_changed(int i) {
  // made attribute list
  BrowserClass * cl = (BrowserClass *) nodes.at(i);
  
  cl->get_attrs(attributes);
  attributes.sort();
  
  // fill table
  atbl->setNumRows(0);
  atbl->setNumRows(attributes.count());
  
  BrowserNode * at;
  int index;
  QValueList<SlotAttr> attrs = inst->attributes;
  
  for (at = attributes.first(), index = 0;
       at != 0;
       at = attributes.next(), index += 1) {
    atbl->setItem(index, 0, new TableItem(atbl, QTableItem::Never, at->get_name()));
    atbl->setItem(index, 1, new TableItem(atbl, QTableItem::Never, ((BrowserNode *) at->parent())->get_name()));
    
    QValueList<SlotAttr>::Iterator it_attr;

    for (it_attr = attrs.begin(); it_attr != attrs.end(); ++it_attr) {
      if ((*it_attr).att == at) {
	atbl->setText(index, 2, (*it_attr).value);
	break;
      }
    }
    
    if (it_attr == attrs.end())
      atbl->setText(index, 2, QString::null);
  }
  
  atbl->setColumnStretchable (2, TRUE);
  atbl->adjustColumn(0);
  atbl->adjustColumn(1);

  if (rtbl)
    rtbl->setEnabled(cl == inst->cl);
}

void ClassInstanceDialog::accept() {
  if (!kvtable->check_unique())
    return;
  
  atbl->forceUpdateCells();
  
  BrowserClassInstance * bn = (BrowserClassInstance *) inst->get_browser_node();
  
  bn->set_name(edname->text().stripWhiteSpace());

  bool newst = bn->get_data()->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
  BrowserClass * new_cl = (BrowserClass *) nodes.at(edtype->currentItem());
  
  if (new_cl != inst->cl)
    inst->set_class(new_cl);
  else if (rtbl) {
    QValueList<SlotRel> rels = inst->get_relations(); // copy !
    QValueList<SlotRel>::Iterator it;
    int row = 0;
    QString yes = TR("Yes");
    
    for (row = 0, it = rels.begin(); it != rels.end(); ++it, row += 1) {
      if (rtbl->text(row, 5).stripWhiteSpace() == yes) {
	SlotRel & sr = *it;
	
	inst->replace(sr.value, sr.rel, 0, sr.is_a, FALSE);
      }
    }
  }
    
  bn->set_comment(comment->text());
  UmlWindow::update_comment_if_needed(bn);
  
  inst->attributes.clear();
  
  int n = atbl->numRows();
  int index;
  
  for (index = 0; index != n; index += 1) {
    QString s = atbl->text(index, 2).stripWhiteSpace();
    
    if (! s.isEmpty())
      inst->attributes.append(SlotAttr((BrowserAttribute *) attributes.at(index),
				       s));
  }
      
  kvtable->update(bn);
  
  ProfiledStereotypes::modified(bn, newst);
  
  bn->modified();
  bn->package_modified();
  inst->modified();
  
  QDialog::accept();
}
