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
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qptrdict.h>

#include "RelatedElementsDialog.h"
#include "DialogUtil.h"
#include "UmlDesktop.h"
#include "BrowserClass.h"
#include "BrowserRelation.h"
#include "BrowserSimpleRelation.h"
#include "RelationData.h"
#include "SimpleRelationData.h"
#include "translate.h"

QSize RelatedElementsDialog::previous_size;

RelatedElementsDialog::RelatedElementsDialog(BrowserNode * e, const char * what,
					     bool inh, bool assoc, BrowserNodeList & l)
    : QDialog(0, "Related elements dialog", TRUE), elt(e), elts(l) {
  setCaption(TR("Related elements dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  
  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);  
  hbox->addWidget(new QLabel(TR("Search for : "), this));
  referenced_rb = new QCheckBox(TR("referenced elements  "), this);
  referenced_rb->setChecked(TRUE);
  hbox->addWidget(referenced_rb);
  referencing_rb = new QCheckBox(TR("referencing elements  "), this);
  hbox->addWidget(referencing_rb);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);  
  hbox->addWidget(new QLabel(TR("Through relations : "), this));
  if (inh) {
    inh_rb = new QCheckBox(TR("generalization  "), this);
    inh_rb->setChecked(TRUE);
    hbox->addWidget(inh_rb);
  }
  else
    inh_rb = 0;
  dep_rb = new QCheckBox(TR("dependency  "), this);
  dep_rb->setChecked(TRUE);
  hbox->addWidget(dep_rb);
  if (assoc) {
    assoc_rb = new QCheckBox(TR("other"), this);
    assoc_rb->setChecked(TRUE);
    hbox->addWidget(assoc_rb);
  }
  else
    assoc_rb = 0;
  hbox->addWidget(new QLabel(this), 1000);
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);  
  sametype_rb = new QCheckBox(TR("Search only for %1", what), this);
  sametype_rb->setChecked(TRUE);
  hbox->addWidget(sametype_rb);
  hbox->addWidget(new QLabel(this), 1000);
  hbox->addWidget(new QLabel(TR("     Search on "), this));
  sb_level = new QSpinBox(1, 10, 1, this, TR("levels"));
  sb_level->setValue(1);
  hbox->addWidget(sb_level);
  hbox->addWidget(new QLabel(TR("levels"), this));

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

void RelatedElementsDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

RelatedElementsDialog::~RelatedElementsDialog() {
  previous_size = size();
}

void RelatedElementsDialog::accept() {
  bool inh = (inh_rb != 0) && inh_rb->isChecked();
  bool dep = dep_rb->isChecked();
  bool assoc = (assoc_rb != 0) && assoc_rb->isChecked();
  
  if (!inh && !dep && !assoc)
    return;

  bool sametype = sametype_rb->isChecked();
  UmlCode k = elt->get_type();
  
  elts.clear();

  QPtrDict<BrowserNode> d(71);
  BrowserNodeList added;
  int lvl = sb_level->value();
    
  added.append(elt);
  d.insert(elt, elt);
    
  do {
    BrowserNodeList newones;
    QListIterator<BrowserNode> it(added);
    
    for (; it.current() != 0; ++it) {
      BrowserNode * e = (BrowserNode *) it.current();
      BrowserNode * bn;
      
      if (referenced_rb->isChecked()) {
	for (bn = (BrowserNode *) e->firstChild();
	     bn != 0;
	     bn = (BrowserNode *) bn->nextSibling()) {
	  if (! bn->deletedp()) {
	    BrowserNode * target;
	    
	    switch (bn->get_type()) {
	    case UmlAssociation:
	    case UmlDirectionalAssociation:
	    case UmlAggregation:
	    case UmlAggregationByValue:
	    case UmlDirectionalAggregation:
	    case UmlDirectionalAggregationByValue:
	      if (! assoc) continue;
	      target = ((RelationData *) bn->get_data())->get_end_class();
	      break;
	    case UmlDependency:
	      if (! dep) continue;
	      target = ((RelationData *) bn->get_data())->get_end_class();
	      break;
	    case UmlGeneralisation:
	    case UmlRealize:
	      if (! inh) continue;
	      target = ((RelationData *) bn->get_data())->get_end_class();
	      break;
	    case UmlDependOn:
	      if (! dep) continue;
	      target = ((SimpleRelationData *) bn->get_data())->get_end_node();
	      break;
	    case UmlInherit:
	      if (! inh) continue;
	      target = ((SimpleRelationData *) bn->get_data())->get_end_node();
	      break;
	    default:
	      continue;
	    }
	    
	    if ((sametype && (target->get_type() != k)) ||
		(d[target] != 0))
	      continue;
	    d.insert(target, target);
	    newones.append(target);
	  }
	}
      }
      
      if (referencing_rb->isChecked()) {
	if (e->get_type() == UmlClass)
	  BrowserRelation::get_relating(e, d, newones, inh, dep, assoc);
	BrowserSimpleRelation::get_relating(e, d, newones, inh, dep, sametype, k);
      }
    }
    
    added = newones;
  } while (--lvl != 0);
  
  d.remove(elt);
  
  QPtrDictIterator<BrowserNode> it(d);
  
  while (it.current()) {
    elts.append(it.current());
    ++it;
  }
  
  QDialog::accept();
}
