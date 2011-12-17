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
#include <qpushbutton.h>
#include <qcombobox.h>

#include "AnnotationDialog.h"
#include "UmlDesktop.h"
#include "BrowserClass.h"
#include "translate.h"

QSize AnnotationDialog::previous_size;

static const char * DefaultAnnotations[] = {
  "Deprecated", "Documented", "Inherit", "Override", "Retention",
  "SuppressWarnings", "Target"
};

AnnotationDialog::AnnotationDialog(QWidget * parent, QString & s, bool visit)
    : QDialog(parent, "annotation editor", TRUE), value(s) {
  QVBoxLayout * vbox = new QVBoxLayout(this);

  vbox->setMargin(5);

  // multiline edit
  
  e = new MultiLineEdit(this);
  e->setText(s);  
  vbox->addWidget(e);
  
  if (! visit) {
    e->setFocus();

    // to choose and add an annotation
    
    QLabel * label =
      new QLabel(TR("\nTo add an annotation at the cursor position\n"
		    "you may select it in the list and press 'add'\n"),
		 this);
    label->setAlignment(Qt::AlignCenter);
    vbox->addWidget(label);
    
    QHBoxLayout * hbox = new QHBoxLayout(vbox);
    QPushButton * add_button;
    
    hbox->setMargin(5);
    add_button = new QPushButton(TR("Add "), this);
    hbox->addWidget(add_button);  
    connect(add_button, SIGNAL(clicked()), this, SLOT(add_annotation()));
    
    cb = new QComboBox(FALSE, this);
    
    QSizePolicy sp = cb->sizePolicy();
    
    sp.setHorData(QSizePolicy::Expanding);
    cb->setSizePolicy(sp);
    cb->setAutoCompletion(completion());
    
    for (int i = 0;
	 i != sizeof(DefaultAnnotations)/sizeof(*DefaultAnnotations);
	 i += 1)
      cb->insertItem(DefaultAnnotations[i]);
    
    QStringList list;
    
    BrowserClass::instances(annotations, "@interface");
    if (! annotations.isEmpty()) {
      annotations.full_names(list);
      cb->insertStringList(list);
    }
    
    hbox->addWidget(cb);
    
    // buttons ok, cancel
    
    vbox->addWidget(new QLabel("", this));
    
    hbox = new QHBoxLayout(vbox);
    hbox->setMargin(5);
    QPushButton * accept = new QPushButton(TR("&OK"), this);
    QPushButton * cancel = new QPushButton(TR("&Cancel"), this);
    QSize bs(cancel->sizeHint());
    
    accept->setDefault( TRUE );
    accept->setFixedSize(bs);
    cancel->setFixedSize(bs);
    
    hbox->addWidget(accept);
    hbox->addWidget(cancel);
    
    connect(accept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
  }
  else {
    e->setReadOnly(TRUE);

    // buttons cancel
    
    vbox->addWidget(new QLabel("", this));
    
    QHBoxLayout * hbox = new QHBoxLayout(vbox);
    
    hbox->setMargin(5);
    QPushButton * close = new QPushButton(TR("&Close"), this);

    hbox->addWidget(close);
    
    connect(close, SIGNAL(clicked()), this, SLOT(reject()));
  }
  
  // not done in polish else the initial size is too small
  UmlDesktop::setsize_center(this, previous_size, 0.3, 0.3);
}

AnnotationDialog::~AnnotationDialog() {
  previous_size = size();
}

void AnnotationDialog::add_annotation() {
  const int ndefault = 
    sizeof(DefaultAnnotations)/sizeof(*DefaultAnnotations);
  int added_index = cb->currentItem();
  QString added;
  
  if (added_index < ndefault)
    added = DefaultAnnotations[added_index];
  else {
    BrowserNode * cl = annotations.at(added_index - ndefault);
    added = cl->get_name();
    
    // have member ?
    int n = 0;
    QString first_name;
    QListViewItem * child;
      
    for (child = cl->firstChild(); child != 0; child = child->nextSibling()) {
      if (! ((BrowserNode *) child)->deletedp()) {
	switch (((BrowserNode *) child)->get_type()) {
	case UmlClass:
	case UmlExtraMember:
	  break;
	default:
	  switch (++n) {
	  case 1:
	    first_name = ((BrowserNode *) child)->get_name();
	    break;
	  case 2:
	    added += "(" + first_name + QString("=, ")
	      + ((BrowserNode *) child)->get_name() + "=";
	    break;
	  default:
	    added += QString(", ") + ((BrowserNode *) child)->get_name() + QString("=");
	  }
	}
      }
    }
    
    switch (n) {
    case 0:
      break;
    case 1:
      added += "()";
      break;
    default:
      added += ")";
    }
  }
  
  e->insert("@" + added + "\n");
  e->setFocus();
}

void AnnotationDialog::accept() {
  value = e->text().stripWhiteSpace();
  
  if (!value.isEmpty())
    value += '\n';
  
  QDialog::accept();
}
