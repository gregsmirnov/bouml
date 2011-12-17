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
#include <qvbox.h> 

#include "UseCaseDialog.h"
#include "BrowserUseCase.h"
#include "UseCaseData.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "KeyValueTable.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "strutil.h"
#include "ProfiledStereotypes.h"
#include "translate.h"

QSize UseCaseDialog::previous_size;

UseCaseDialog::UseCaseDialog(UseCaseData * u)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), uc(u) {
  BrowserNode * bn = u->browser_node;
  
  bn->edit_start();
  
  if (bn->is_writable()) {
    setOkButton(TR("OK"));
    setCancelButton(TR("Cancel"));
  }
  else {
    setOkButton(QString::null);
    setCancelButton(TR("Close"));
  }

  setCaption(TR("Use Case dialog"));
  
  bool visit = !hasOkButton();
  
  // general tab
  
  QGrid * grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);

  new QLabel(TR("name : "), grid);
  edname = new LineEdit(bn->get_name(), grid);
  edname->setReadOnly(visit);

  new QLabel(TR("stereotype : "), grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(uc->get_stereotype()));
  if (! visit) {
    edstereotype->insertStringList(BrowserUseCase::default_stereotypes());
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlUseCase));
    edstereotype->setAutoCompletion(completion());
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  new QLabel(TR("extension \npoints : "), grid);
  extension_points = new MultiLineEdit(grid);
  extension_points->setReadOnly(visit);
  extension_points->setText(uc->get_extension_points());
  QFont font = extension_points->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  extension_points->setFont(font);

  QVBox * vtab = new QVBox(grid);
  new QLabel(TR("description :"), vtab);
  if (! visit)
    connect(new SmallPushButton(TR("Editor"), vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(bn->get_comment());
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(bn, grid, visit);
  addTab(grid, TR("Properties"));
  
  open_dialog(this);
}

void UseCaseDialog::polish() {
  QTabDialog::polish();  
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

UseCaseDialog::~UseCaseDialog() {
  uc->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
  
  close_dialog(this);
}

void UseCaseDialog::edit_description() {
  edit(comment->text(), 
       edname->text().stripWhiteSpace() + "_description",
       uc, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void UseCaseDialog::post_edit_description(UseCaseDialog * d, QString s)
{
  d->comment->setText(s);
}

void UseCaseDialog::accept() {
  if (!check_edits(edits) || !kvtable->check_unique())
    return;
    
  QString s = edname->text().stripWhiteSpace();
  BrowserNode * bn = uc->browser_node;
  
  if ((s != bn->get_name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlUseCase,
						       bn->allow_spaces(),
						       bn->allow_empty())) {
    msg_critical(TR("Error"), edname->text() + TR("\n\nillegal name or already used"));
    return;
  }
  else
    bn->set_name(s);

  bool newst = uc->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
  
  bn->set_comment(comment->text());
  UmlWindow::update_comment_if_needed(bn);
    
  kvtable->update(bn);
  
  uc->set_extension_points(extension_points->text().stripWhiteSpace());
  
  ProfiledStereotypes::modified(bn, newst);
  
  bn->modified();
  bn->package_modified();
  uc->modified();
    
  QTabDialog::accept();
}
