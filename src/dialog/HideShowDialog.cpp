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
#include <qlistbox.h>
#include <qcheckbox.h>

#include "HideShowDialog.h"
#include "BrowserNode.h"
#include "ClassMemberData.h"
#include "DialogUtil.h"
#include "ListBoxBrowserNode.h"
#include "UmlDesktop.h"
#include "translate.h"

QSize HideShowDialog::previous_size;

HideShowDialog::HideShowDialog(const BrowserNodeList & a,
			       QValueList<BrowserNode *> & hv,
			       BooL & visible)
    : QDialog(0, "hide/show dialog", TRUE), all(a),
      hidden_visible(hv), on_visible(visible) {
  setCaption(TR("Settings dialog"));
  
  QVBoxLayout * vbox = new QVBoxLayout(this);  
  QHBoxLayout * hbox;
  QVBoxLayout * subvbox;
  QPushButton * button;

  vbox->setMargin(5);
  
  hbox = new QHBoxLayout(vbox);
  subvbox = new QVBoxLayout(hbox);
  subvbox->setMargin(5);
  subvbox->addWidget(new QLabel(TR("Visible"), this), 0, AlignHCenter);
  subvbox->addWidget(lb_visible = new QListBox(this));
  lb_visible->setSelectionMode(QListBox::Multi);
  
  subvbox = new QVBoxLayout(hbox);
  subvbox->setResizeMode(QLayout::Minimum);
  subvbox->setMargin(5);
  //subvbox->addWidget(new QLabel("", this));
  subvbox->addStretch(100);
  subvbox->addWidget(button = new SmallPushButton(" -> ", this));
  connect(button, SIGNAL(clicked()), this, SLOT(hide_them()));
  subvbox->addStretch(100);
  subvbox->addWidget(button = new SmallPushButton(" <- ", this));
  connect(button, SIGNAL(clicked()), this, SLOT(show_them()));
  subvbox->addStretch(100);
  
  subvbox = new QVBoxLayout(hbox);
  subvbox->setMargin(5);
  subvbox->addWidget(new QLabel(TR("Hidden"), this), 0, AlignHCenter);
  subvbox->addWidget(lb_hidden = new QListBox(this));
  lb_hidden->setSelectionMode(QListBox::Multi);
  
  QListIterator<BrowserNode> it(all);
  
  while (it.current() != 0) {
    QString def = it.current()->get_data()->definition(TRUE, FALSE);
    
    (((hidden_visible.findIndex(it.current()) == -1) ^ on_visible)
     ? lb_visible : lb_hidden)
      ->insertItem(new ListBoxBrowserNode(it.current(), def));
    
    ++it;
  }
  
  lb_visible->sort();
  lb_hidden->sort();
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * sh_all = new SmallPushButton(TR("Show all"), this);
  QPushButton * hi_all = new SmallPushButton(TR("Hide all"), this);
  QPushButton * hi_priv = new SmallPushButton(TR("Hide private ones"), this);
  QPushButton * hi_priv_prot = new SmallPushButton(TR("Hide private and\nprotected ones"), this);
  QSize bs(hi_priv_prot->sizeHint());
  
  sh_all->setFixedSize(bs);
  hi_all->setFixedSize(bs);
  hi_priv->setFixedSize(bs);
  hi_priv_prot->setFixedSize(bs);
  
  hbox->addWidget(sh_all);
  hbox->addWidget(hi_all);
  hbox->addWidget(hi_priv);
  hbox->addWidget(hi_priv_prot);
  
  connect(sh_all, SIGNAL(clicked()), this, SLOT(show_all()));
  connect(hi_all, SIGNAL(clicked()), this, SLOT(hide_all()));  
  connect(hi_priv, SIGNAL(clicked()), this, SLOT(hide_private()));  
  connect(hi_priv_prot, SIGNAL(clicked()), this, SLOT(hide_private_protected()));  
  
  //
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);

  cb_visible = new QCheckBox(TR("Specify visible members rather than hidden ones"), this);
  cb_visible->setChecked(on_visible);
  hbox->addWidget(cb_visible);
    
  //
  
  hbox = new QHBoxLayout(vbox); 
  hbox->setMargin(5);
  QPushButton * ok = new QPushButton(TR("&OK"), this);
  QPushButton * cancel = new QPushButton(TR("&Cancel"), this);
    
  ok->setDefault( TRUE );
  
  bs = cancel->sizeHint();
  ok->setFixedSize(bs);
  cancel->setFixedSize(bs);
  
  hbox->addWidget(ok);
  hbox->addWidget(cancel);
    
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));  
}

void HideShowDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

HideShowDialog::~HideShowDialog() {
  previous_size = size();
}

void HideShowDialog::hide_them() {
  unsigned int i = 0;
  
  while (i < lb_visible->count()) {
    QListBoxItem * item = lb_visible->item(i);
    if (item->selected()) {
      lb_visible->takeItem(item);
      lb_hidden->insertItem(item);
    }
    else
      i += 1;
  }
}

void HideShowDialog::show_them() {
  unsigned int i = 0;
  
  while (i < lb_hidden->count()) {
    QListBoxItem * item = lb_hidden->item(i);
    
    if (item->selected()) {
      lb_hidden->takeItem(item);
      lb_visible->insertItem(item);
    }
    else
      i += 1;
  }
}

void HideShowDialog::show_all() {
  while (lb_hidden->count()) {
    QListBoxItem * item = lb_hidden->firstItem();
    
    lb_hidden->takeItem(item);
    lb_visible->insertItem(item);
  }
}

void HideShowDialog::hide_all() {
  while (lb_visible->count()) {
    QListBoxItem * item = lb_visible->firstItem();
    
    lb_visible->takeItem(item);
    lb_hidden->insertItem(item);
  }
}

void HideShowDialog::hide_private() {
  lb_visible->clear();
  lb_hidden->clear();
  
  QListIterator<BrowserNode> it(all);
  
  while (it.current() != 0) {
    BasicData * m = it.current()->get_data();
    QString def = m->definition(TRUE, FALSE);
    
    ((((ClassMemberData *) m)->get_visibility(m->get_browser_node()) != UmlPrivate)
     ? lb_visible : lb_hidden)
      ->insertItem(new ListBoxBrowserNode(it.current(), def));
    
    ++it;
  }
  
  lb_visible->sort();
  lb_hidden->sort();
}

void HideShowDialog::hide_private_protected() {
  lb_visible->clear();
  lb_hidden->clear();
  
  QListIterator<BrowserNode> it(all);
  
  while (it.current() != 0) {
    BasicData * m = it.current()->get_data();
    QString def = m->definition(TRUE, FALSE);    
    UmlVisibility visi =
      ((ClassMemberData *) m)->get_visibility(m->get_browser_node());
    
    (((visi == UmlPublic) || (visi == UmlPackageVisibility))
     ? lb_visible : lb_hidden)
      ->insertItem(new ListBoxBrowserNode(it.current(), def));
    
    ++it;
  }
  
  lb_visible->sort();
  lb_hidden->sort();
}

void HideShowDialog::accept() {
  hidden_visible.clear();
  on_visible = cb_visible->isChecked();
  
  QListBox * lb = (on_visible) ? lb_visible : lb_hidden;
  
  for (unsigned int i = 0; i != lb->count(); i += 1)
    hidden_visible.append(((ListBoxBrowserNode *) lb->item(i))->browser_node);
  
  QDialog::accept();
}
