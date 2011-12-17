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
#include <qpushbutton.h>
#include <qcombobox.h> 
#include <qcheckbox.h> 

#include "ComponentDialog.h"
#include "BrowserComponent.h"
#include "SimpleData.h"
#include "BrowserClass.h"
#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "KeyValueTable.h"
#include "ListBoxBrowserNode.h"
#include "strutil.h"
#include "UmlPixmap.h"
#include "BodyDialog.h"
#include "ProfiledStereotypes.h"
#include "translate.h"

QSize ComponentDialog::previous_size;

ComponentDialog::ComponentDialog(SimpleData * nd)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), data(nd) {
  nd->get_browser_node()->edit_start();
  
  if (nd->get_browser_node()->is_writable()) {
    setOkButton(TR("OK"));
    setCancelButton(TR("Cancel"));
  }
  else {
    setOkButton(QString::null);
    setCancelButton(TR("Close"));
  }

  setCaption(TR("component dialog"));
    
  BrowserComponent * bn = (BrowserComponent *) nd->get_browser_node();
    
  init_uml_tab();
  rqs = bn->get_required_classes();
  init_l_tab(rq_page, rq_stereotypefilter, &ComponentDialog::rq_stereotypeFilterActivated,
	     SLOT(rq_stereotypeFilterActivated(const QString &)),
	     SLOT(require_cls()), SLOT(unrequire_cls()), 
	     lb_rq_available, lb_rq, rqs, TR("Required classes"));
  prs = bn->get_provided_classes();
  init_l_tab(pr_page, pr_stereotypefilter, &ComponentDialog::pr_stereotypeFilterActivated,
	     SLOT(pr_stereotypeFilterActivated(const QString &)),
	     SLOT(provide_cls()), SLOT(unprovide_cls()), 
	     lb_pr_available, lb_pr, prs, TR("Provided classes"));
  rzs = bn->get_realizing_classes();
  init_l_tab(rz_page, rz_stereotypefilter, &ComponentDialog::rz_stereotypeFilterActivated,
	     SLOT(rz_stereotypeFilterActivated(const QString &)),
	     SLOT(realize_cls()), SLOT(unrealize_cls()), 
	     lb_rz_available, lb_rz, rzs, TR("Realizing classes"));

  // USER : list key - value
  
  QGrid * grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable((BrowserComponent *) data->get_browser_node(),
			       grid, !hasOkButton());
  addTab(grid, TR("Properties"));
  
  //
    
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(change_tabs(QWidget *)));
  
  open_dialog(this);
}

void ComponentDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::setsize_center(this, previous_size, 0.8, 0.8);
}

ComponentDialog::~ComponentDialog() {
  data->get_browser_node()->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
  
  close_dialog(this);
}
    
void ComponentDialog::change_tabs(QWidget * w) {
  if (hasOkButton() && (w == umltab))
    edname->setFocus();
}

void ComponentDialog::init_uml_tab() {
  bool visit = !hasOkButton();  
  
  BrowserComponent * bn = (BrowserComponent *) data->get_browser_node();
  QVBox * vbox;
  QGrid * grid = new QGrid(2, this);
  
  umltab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);

  new QLabel(TR("name : "), grid);
  edname = new LineEdit(bn->get_name(), grid);
  edname->setReadOnly(visit);
    
  new QLabel(TR("stereotype : "), grid);
  edstereotype = new QComboBox(TRUE, grid);
  edstereotype->insertItem(toUnicode(data->get_stereotype()));
  if (! visit) {
    edstereotype->insertStringList(BrowserComponent::default_stereotypes());
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlComponent));
    edstereotype->setAutoCompletion(completion());
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
    
  vbox = new QVBox(grid);
  new QLabel(TR("description :"), vbox);
  if (! visit)
    connect(new SmallPushButton(TR("Editor"), vbox), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  QFont font = comment->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  comment->setFont(font);
  
  addTab(grid, "Uml");
}

void ComponentDialog::init_l_tab(QVBox *& page, QComboBox *& stereotypefilter,
				 void (ComponentDialog::* filteractivated)(const QString & st),
				 const char * slt,
				 const char * add_slt, const char * remove_slt,
				 QListBox *& lb_available, QListBox *& lb,
				 const QValueList<BrowserClass *> & cls,
				 const char * lbl) {
  bool visit = !hasOkButton();  
  QHBox * hbox;
  QVBox * vbox;  
  QPushButton * button;
  
  page = new QVBox(this);
  
  if (!visit) {
    hbox = new QHBox(page);
    hbox->setMargin(5);
    new QLabel(TR("Stereotype filtering  "), hbox);
    stereotypefilter = new QComboBox(TRUE, hbox);
    stereotypefilter->setAutoCompletion(completion());
    stereotypefilter->insertItem("");
    stereotypefilter->insertStringList(BrowserClass::default_stereotypes());
    stereotypefilter->insertStringList(ProfiledStereotypes::defaults(UmlComponent));
    stereotypefilter->setCurrentItem(0);
    QSizePolicy sp = stereotypefilter->sizePolicy();
    sp.setHorData(QSizePolicy::Expanding);
    stereotypefilter->setSizePolicy(sp);
    connect(stereotypefilter, SIGNAL(activated(const QString &)),
	    this, slt);
    
    hbox = new QHBox(page);
    vbox = new QVBox(hbox);
    vbox->setMargin(5);
    (new QLabel(TR("Available classes"), vbox))->setAlignment(AlignCenter);
    lb_available = new QListBox(vbox);
    lb_available->setSelectionMode(QListBox::Multi);
    
    vbox = new QVBox(hbox);
    vbox->setMargin(5);
    (new QLabel("", vbox))->setScaledContents(TRUE);
    button = new QPushButton(vbox);
    button->setPixmap(*rightPixmap);
    connect(button, SIGNAL(clicked()), this, add_slt);
    (new QLabel("", vbox))->setScaledContents(TRUE);
    button = new QPushButton(vbox);
    button->setPixmap(*leftPixmap);
    connect(button, SIGNAL(clicked()), this, remove_slt);
    (new QLabel("", vbox))->setScaledContents(TRUE);
    vbox = new QVBox(hbox);
  }
  else
    vbox = page;
  
  vbox->setMargin(5);
  (new QLabel(lbl, vbox))->setAlignment(AlignCenter);
  lb = new QListBox(vbox);
  lb->setSelectionMode((visit) ? QListBox::NoSelection
			       : QListBox::Multi);
  
  QValueList<BrowserClass *>::ConstIterator it;
  
  for (it = cls.begin(); it != cls.end(); ++it)
    if (!(*it)->deletedp())
      lb->insertItem(new ListBoxBrowserNode(*it, (*it)->full_name(TRUE)));
  
  lb->sort();

  if (! visit)
    (this->*filteractivated)(QString::null);
    
  addTab(page, lbl);
}

void ComponentDialog::rq_stereotypeFilterActivated(const QString & st) {
  lb_rq_available->clear();
  
  BrowserNodeList classes;
  BrowserNode * cl;

  BrowserClass::instances(classes, st, TRUE);
  
  for (cl = classes.first(); cl != 0; cl = classes.next())
    if (rqs.findIndex((BrowserClass *) cl) == -1)
      lb_rq_available->insertItem(new ListBoxBrowserNode(cl, cl->full_name(TRUE)));
  
  lb_rq_available->sort();
}

void ComponentDialog::pr_stereotypeFilterActivated(const QString & st) {
  lb_pr_available->clear();
  
  BrowserNodeList classes;
  BrowserNode * cl;

  BrowserClass::instances(classes, st, TRUE);
  
  for (cl = classes.first(); cl != 0; cl = classes.next())
    if (prs.findIndex((BrowserClass *) cl) == -1)
      lb_pr_available->insertItem(new ListBoxBrowserNode(cl, cl->full_name(TRUE)));
  
  lb_pr_available->sort();
}

void ComponentDialog::rz_stereotypeFilterActivated(const QString & st) {
  lb_rz_available->clear();
  
  BrowserNodeList classes;
  BrowserNode * cl;

  BrowserClass::instances(classes, st, TRUE);
  
  for (cl = classes.first(); cl != 0; cl = classes.next())
    if (rzs.findIndex((BrowserClass *) cl) == -1)
      lb_rz_available->insertItem(new ListBoxBrowserNode(cl, cl->full_name(TRUE)));
  
  lb_rz_available->sort();
}

void ComponentDialog::require_cls() {
  unsigned int i = 0;
  
  while (i != lb_rq_available->count()) {
    QListBoxItem * item = lb_rq_available->item(i);
    
    if (item->selected()) {
      rqs.append((BrowserClass *) ((ListBoxBrowserNode *) item)->browser_node);
      lb_rq_available->takeItem(item);
      lb_rq->insertItem(item);
    }
    else
      i += 1;
  }
  
  lb_rq->sort();
}

void ComponentDialog::unrequire_cls() {
  unsigned int i = 0;
  
  while (i != lb_rq->count()) {
    QListBoxItem * item = lb_rq->item(i);
    
    if (item->selected()) {
      BrowserNode * c = ((ListBoxBrowserNode *) item)->browser_node;
      QString s = item->text();
      
      delete item;
      rqs.remove((BrowserClass *) c);
      
      if (rq_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == rq_stereotypefilter->currentText()))
	lb_rq_available->insertItem(new ListBoxBrowserNode(c, s));
    }
    else
      i += 1;
  }
  
  lb_rq_available->sort();
}

void ComponentDialog::provide_cls() {
  unsigned int i = 0;
  
  while (i != lb_pr_available->count()) {
    QListBoxItem * item = lb_pr_available->item(i);
    
    if (item->selected()) {
      prs.append((BrowserClass *) ((ListBoxBrowserNode *) item)->browser_node);
      lb_pr_available->takeItem(item);
      lb_pr->insertItem(item);
    }
    else
      i += 1;
  }
  
  lb_pr->sort();
}

void ComponentDialog::unprovide_cls() {
  unsigned int i = 0;
  
  while (i != lb_pr->count()) {
    QListBoxItem * item = lb_pr->item(i);
    
    if (item->selected()) {
      BrowserNode * c = ((ListBoxBrowserNode *) item)->browser_node;
      QString s = item->text();
      
      delete item;
      prs.remove((BrowserClass *) c);
      
      if (pr_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == pr_stereotypefilter->currentText()))
	lb_pr_available->insertItem(new ListBoxBrowserNode(c, s));
    }
    else
      i += 1;
  }
  
  lb_pr_available->sort();
}

void ComponentDialog::realize_cls() {
  unsigned int i = 0;
  
  while (i != lb_rz_available->count()) {
    QListBoxItem * item = lb_rz_available->item(i);
    
    if (item->selected()) {
      rzs.append((BrowserClass *) ((ListBoxBrowserNode *) item)->browser_node);
      lb_rz_available->takeItem(item);
      lb_rz->insertItem(item);
    }
    else
      i += 1;
  }
  
  lb_rz->sort();
}

void ComponentDialog::unrealize_cls() {
  unsigned int i = 0;
  
  while (i != lb_rz->count()) {
    QListBoxItem * item = lb_rz->item(i);
    
    if (item->selected()) {
      BrowserNode * c = ((ListBoxBrowserNode *) item)->browser_node;
      QString s = item->text();
      
      delete item;
      rzs.remove((BrowserClass *) c);
      
      if (rz_stereotypefilter->currentText().isEmpty() ||
	  (c->get_stereotype() == rz_stereotypefilter->currentText()))
	lb_rz_available->insertItem(new ListBoxBrowserNode(c, s));
    }
    else
      i += 1;
  }
  
  lb_rz_available->sort();
}

void ComponentDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       data, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ComponentDialog::post_edit_description(ComponentDialog * d, QString s) {
  d->comment->setText(s);
}

void ComponentDialog::accept() {
  if (!check_edits(edits) || !kvtable->check_unique())
    return;
    
  QString s = edname->text().stripWhiteSpace();
  BrowserComponent * bn = (BrowserComponent *) data->get_browser_node();
  
  if ((s != bn->get_name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, bn->get_type(),
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical(TR("Error"), edname->text() + TR("\n\nillegal name or already used"));
  else {  
    bn->set_name(s);
    bn->set_comment(comment->text());
    UmlWindow::update_comment_if_needed(bn);
            
    bool newst = data->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    QValueList<BrowserClass *> rq;
    QValueList<BrowserClass *> rz;
    QValueList<BrowserClass *> pr;
    
    unsigned int i;
    unsigned int n;
    
    for (i = 0, n = lb_rq->count(); i != n; i += 1)
      rq.append((BrowserClass *) ((ListBoxBrowserNode *) lb_rq->item(i))->browser_node);
    for (i = 0, n = lb_pr->count(); i != n; i += 1)
      pr.append((BrowserClass *) ((ListBoxBrowserNode *) lb_pr->item(i))->browser_node);
    for (i = 0, n = lb_rz->count(); i != n; i += 1)
      rz.append((BrowserClass *) ((ListBoxBrowserNode *) lb_rz->item(i))->browser_node);
    
    bn->set_associated_classes(rz, pr, rq);
    
    kvtable->update(bn);
  
    ProfiledStereotypes::modified(bn, newst);
    
    bn->modified();
    bn->package_modified();
    data->modified();
    
    QTabDialog::accept();
  }
}
