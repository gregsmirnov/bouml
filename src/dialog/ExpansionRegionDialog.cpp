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
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h> 
#include <qcheckbox.h> 
#include <qvbox.h> 

#include "ExpansionRegionDialog.h"
#include "ExpansionRegionData.h"
#include "BrowserExpansionRegion.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "KeyValueTable.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "strutil.h"
#include "ProfiledStereotypes.h"
#include "translate.h"

QSize ExpansionRegionDialog::previous_size;

ExpansionRegionDialog::ExpansionRegionDialog(ExpansionRegionData * nd)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), data(nd) {
  nd->browser_node->edit_start();
  
  if (nd->browser_node->is_writable()) {
    setOkButton(TR("OK"));
    setCancelButton(TR("Cancel"));
  }
  else {
    setOkButton(QString::null);
    setCancelButton(TR("Close"));
  }

  setCaption(TR("Expansion Region dialog"));
  
  bool visit = !hasOkButton();
  
  // general tab
  
  BrowserNode * bn = data->get_browser_node();
  QGrid * grid = new QGrid(2, this);

  umltab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);

  new QLabel(TR("name : "), grid);
  edname = new LineEdit(bn->get_name(), grid);
  edname->setReadOnly(visit);
    
  new QLabel(TR("stereotype : "), grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(data->get_stereotype()));
  if (! visit) {
    edstereotype->insertStringList(BrowserExpansionRegion::default_stereotypes());
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlExpansionRegion));
    edstereotype->setAutoCompletion(completion());
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);

  QHBox * htab;

  new QLabel(TR("mode :"), grid);
  htab = new QHBox(grid);
  edmode = new QComboBox(FALSE, htab);
  
  UmlExpansionKind m = data->get_mode();
  
  edmode->insertItem(stringify(m));
  if (! visit) {
    if (m != UmlParallel)
      edmode->insertItem(stringify(UmlParallel));
    if (m != UmlIterative)
      edmode->insertItem(stringify(UmlIterative));
    if (m != UmlStream)
      edmode->insertItem(stringify(UmlStream));
  }
  
  new QLabel("  ", htab);
  must_isolate_cb = new QCheckBox(TR("must isolate"), htab);
  if (data->must_isolate)
    must_isolate_cb->setChecked(TRUE);
  must_isolate_cb->setDisabled(visit);
  new QLabel("", htab);

  QVBox * vtab = new QVBox(grid);
  new QLabel(TR("description :"), vtab);
  if (! visit)
    connect(new SmallPushButton(TR("Editor"), vtab), SIGNAL(clicked()),
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
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(bn, grid, visit);
  addTab(grid, TR("Properties"));
  
  //
    
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(change_tabs(QWidget *)));
  
  open_dialog(this);
}

void ExpansionRegionDialog::polish() {
  QTabDialog::polish();  
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

ExpansionRegionDialog::~ExpansionRegionDialog() {
  data->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
  
  close_dialog(this);
}
    
void ExpansionRegionDialog::change_tabs(QWidget * w) {
  if (hasOkButton() && (w == umltab))
    edname->setFocus();
}

void ExpansionRegionDialog::edit_description() {
  edit(comment->text(), 
       (edname == 0) ? QString(TR("description"))
		     : edname->text().stripWhiteSpace() + "_description",
       data, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ExpansionRegionDialog::post_edit_description(ExpansionRegionDialog * d, QString s)
{
  d->comment->setText(s);
}

void ExpansionRegionDialog::accept() {
  if (!check_edits(edits) || !kvtable->check_unique())
    return;
    
  BrowserNode * bn = data->get_browser_node();

  if (edname != 0) {
    QString s = edname->text().stripWhiteSpace();

    if ((s != bn->get_name()) &&
	((BrowserNode *) bn->parent())->wrong_child_name(s, bn->get_type(),
							 bn->allow_spaces(),
							 bn->allow_empty())) {
      msg_critical(TR("Error"), edname->text() + TR("\n\nillegal name or already used"));
      return;
    }
    else
      bn->set_name(s);
  }

  data->must_isolate = must_isolate_cb->isChecked();
  data->mode = expansion_mode_kind(edmode->currentText());

  bool newst = data->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
  
  bn->set_comment(comment->text());
  UmlWindow::update_comment_if_needed(bn);
    
  kvtable->update(bn);
  
  ProfiledStereotypes::modified(bn, newst);
  
  bn->package_modified();
  data->modified();
    
  QTabDialog::accept();
}
