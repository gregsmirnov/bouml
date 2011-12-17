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
#include <qpopupmenu.h> 
#include <qcursor.h> 


#include "ActivityPartitionDialog.h"
#include "ActivityPartitionData.h"
#include "BrowserActivityPartition.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "KeyValueTable.h"
#include "UmlDesktop.h"
#include "BodyDialog.h"
#include "strutil.h"
#include "ProfiledStereotypes.h"
#include "BrowserView.h"
#include "translate.h"

QSize ActivityPartitionDialog::previous_size;

ActivityPartitionDialog::ActivityPartitionDialog(ActivityPartitionData * d)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), data(d) {
  d->browser_node->edit_start();
  
  if (d->browser_node->is_writable()) {
    setOkButton(TR("OK"));
    setCancelButton(TR("Cancel"));
  }
  else {
    setOkButton(QString::null);
    setCancelButton(TR("Close"));
  }

  setCaption(TR("Activity Partition dialog"));
  
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
    edstereotype->insertStringList(BrowserActivityPartition::default_stereotypes());
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlActivityPartition));
    edstereotype->setAutoCompletion(completion());
  }
  edstereotype->setCurrentItem(0);
  QSizePolicy sp = edstereotype->sizePolicy();
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
  
  connect(new SmallPushButton(TR("represents :"), grid), SIGNAL(clicked()),
	  this, SLOT(menu_represents()));
  edrepresents = new QComboBox(FALSE, grid);
  if ((data->represents != 0) && !data->represents->deletedp()) {
    represented = data->represents;
    edrepresents->insertItem(*(data->represents->pixmap(0)), 
			     data->represents->full_name(TRUE));
  }
  else {
    represented = 0;
    edrepresents->insertItem("");
  }
  if (! visit)
    edrepresents->insertItem("");
  edrepresents->setCurrentItem(0);
  edrepresents->setSizePolicy(sp);

  QHBox * htab;

  new QLabel(grid);
  htab = new QHBox(grid);
  new QLabel("  ", htab);
  dimension_cb = new QCheckBox(TR("is dimension"), htab);
  if (data->is_dimension)
    dimension_cb->setChecked(TRUE);
  dimension_cb->setDisabled(visit);
  new QLabel("", htab);
  external_cb = new QCheckBox(TR("is external"), htab);
  if (data->is_external)
    external_cb->setChecked(TRUE);
  external_cb->setDisabled(visit);
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

void ActivityPartitionDialog::polish() {
  QTabDialog::polish();  
  UmlDesktop::limitsize_move(this, previous_size, 0.8, 0.8);
}

ActivityPartitionDialog::~ActivityPartitionDialog() {
  data->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
  
  close_dialog(this);
}
    
void ActivityPartitionDialog::change_tabs(QWidget * w) {
  if (hasOkButton() && (w == umltab))
    edname->setFocus();
}

void ActivityPartitionDialog::edit_description() {
  edit(comment->text(), 
       (edname == 0) ? QString("description")
		     : edname->text().stripWhiteSpace() + "_description",
       data, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void ActivityPartitionDialog::post_edit_description(ActivityPartitionDialog * d, QString s)
{
  d->comment->setText(s);
}

static bool allowed(BrowserNode * bn)
{
  if (!bn->deletedp()) {
    switch (bn->get_type()) {
    case UmlClassView:
    case UmlUseCaseView:
    case UmlComponentView:
    case UmlDeploymentView:
      break;
    case UmlPackage:
      if (bn->parent() == 0)
	// project
	break;
      // no break
    default:
      return TRUE;
    }
  }
  
  return FALSE;
}

void ActivityPartitionDialog::menu_represents() {
  if (!hasOkButton() && (represented == 0))
    return;
  
  QPopupMenu m(0);

  m.insertItem(TR("Choose"), -1);
  m.insertSeparator();
  if (represented != 0)
    m.insertItem(TR("Select in browser"), 0);

  BrowserNode * bn = BrowserView::selected_item();
    
  if ((bn != 0) && allowed(bn))
    m.insertItem(TR("Choose element selected in browser"), 1);
  
  const QList<BrowserNode> & l = BrowserNode::marked_nodes();
  
  if ((l.count() == 1) && allowed(l.getFirst()))
    m.insertItem(TR("Choose element marked in browser"), 2);
  
  switch (m.exec(QCursor::pos())) {
  case 0:
    represented->select_in_browser();
    return;
  case 1:
    represented = bn;
    break;
  case 2:
    represented = l.getFirst();
    break;
  default:
    return;
  }
  
  edrepresents->changeItem(*(represented->pixmap(0)), 
			   represented->full_name(TRUE),
			   0);
}

void ActivityPartitionDialog::accept() {
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

  data->represents = (edrepresents->currentText().isEmpty())
    ? 0 : represented;
  data->is_dimension = dimension_cb->isChecked();
  data->is_external = external_cb->isChecked();

  bool newst = data->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
  
  bn->set_comment(comment->text());
  UmlWindow::update_comment_if_needed(bn);
    
  kvtable->update(bn);
  
  ProfiledStereotypes::modified(bn, newst);
  
  bn->package_modified();
  data->modified();
    
  QTabDialog::accept();
}
