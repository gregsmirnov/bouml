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
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qradiobutton.h> 
#include <qpushbutton.h> 
#include <qpopupmenu.h> 
#include <qcursor.h> 

#include "PinDialog.h"
#include "PinData.h"
#include "BrowserClass.h"
#include "BrowserPin.h"
#include "BrowserView.h"
#include "KeyValueTable.h"
#include "UmlWindow.h"
#include "UmlDesktop.h"
#include "DialogUtil.h"
#include "GenerationSettings.h"
#include "strutil.h"
#include "BodyDialog.h"
#include "ProfiledStereotypes.h"
#include "translate.h"

QSize PinDialog::previous_size;

PinDialog::PinDialog(PinData * pi)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), pin(pi) {
  pi->browser_node->edit_start();
  
  if (pi->browser_node->is_writable()) {
    setOkButton(TR("OK"));
    setCancelButton(TR("Cancel"));
  }
  else {
    setOkButton(QString::null);
    setCancelButton(TR("Close"));
  }

  visit = !hasOkButton();
  setCaption(TR("Pin dialog"));
  
  QGrid * grid;
  QHBox * htab;
  QString s;
    
  // general tab
  
  grid = new QGrid(2, this);
  umltab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);
  
  new QLabel(TR("name :"), grid);
  edname = new LineEdit(pi->name(), grid);
  edname->setReadOnly(visit);

  QFont font = edname->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  
  new QLabel(TR("stereotype : "), grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(pi->stereotype));
  if (! visit) {
    edstereotype->insertStringList(BrowserPin::default_stereotypes());
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlActivityPin));
    edstereotype->setAutoCompletion(completion());
  }
  edstereotype->setCurrentItem(0);
  
  QSizePolicy sp = edstereotype->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);
    
  connect(new SmallPushButton(TR("type :"), grid), SIGNAL(clicked()),
	  this, SLOT(menu_type()));
  
  edtype = new QComboBox(!visit, grid);
  edtype->insertItem(pi->get_type().get_full_type());
  if (!visit) {
    BrowserClass::instances(nodes);
    nodes.full_names(list);
    
    edtype->insertStringList(GenerationSettings::basic_types());
    offset = edtype->count();
    edtype->insertStringList(list);
    edtype->setAutoCompletion(completion());
    view = pi->browser_node->container(UmlClass);
  }
  edtype->setCurrentItem(0);
  edtype->setSizePolicy(sp);
  
  new QLabel(TR("direction :"), grid);
  htab = new QHBox(grid);
  eddir = new QComboBox(FALSE, htab);
  
  UmlParamDirection dir = pi->get_dir();
  
  eddir->insertItem(stringify(dir));
  if (! visit) {
    // note : inout not allowed
    if (dir != UmlIn)
      eddir->insertItem(stringify(UmlIn));
    if (dir != UmlOut)
      eddir->insertItem(stringify(UmlOut));
    if (dir != UmlReturn)
      eddir->insertItem(stringify(UmlReturn));
  }
  
  new QLabel(TR("   multiplicity : "), htab);
  edmultiplicity = new QComboBox(!visit, htab);
  edmultiplicity->setSizePolicy(sp);
  edmultiplicity->insertItem(pi->get_multiplicity());
  if (!visit) {
    edmultiplicity->insertItem("1");
    edmultiplicity->insertItem("0..1");
    edmultiplicity->insertItem("*");
    edmultiplicity->insertItem("1..*");
  }
  
  new QLabel(TR("   ordering : "), htab);
  edordering = new QComboBox(FALSE, htab);
  
  UmlOrderingKind o = pi->get_ordering();
  
  edordering->insertItem(stringify(o));
  if (!visit) {
    if (o != UmlUnordered)
      edordering->insertItem(stringify(UmlUnordered));
    if (o != UmlOrdered)
      edordering->insertItem(stringify(UmlOrdered));
    if (o != UmlLifo)
      edordering->insertItem(stringify(UmlLifo));
    if (o != UmlFifo)
      edordering->insertItem(stringify(UmlFifo));
  }
    
  new QLabel(TR("   effect : "), htab);
  edeffect = new QComboBox(FALSE, htab);
  
  UmlParamEffect e = pi->get_effect();
  
  edeffect->insertItem(stringify(e));
  if (!visit) {
    if (e != UmlNoEffect)
      edeffect->insertItem(stringify(UmlNoEffect));
    if (e != UmlCreate)
      edeffect->insertItem(stringify(UmlCreate));
    if (e != UmlRead)
      edeffect->insertItem(stringify(UmlRead));
    if (e != UmlUpdate)
      edeffect->insertItem(stringify(UmlUpdate));
    if (e != UmlDelete)
      edeffect->insertItem(stringify(UmlDelete));
  }
    
  new QLabel(TR("in state : "), grid);
  edin_state = new LineEdit(pi->in_state, grid);
  edin_state->setReadOnly(visit);
       
  new QLabel(grid);
  htab = new QHBox(grid);
  QButtonGroup * bg = 
    new QButtonGroup(2, Qt::Horizontal, QString::null, htab);
  
  is_control_cb = new QCheckBox(TR("is_control"), bg);
  if (pi->is_control)
    is_control_cb->setChecked(TRUE);
  is_control_cb->setDisabled(visit);
  
  unique_cb = new QCheckBox(TR("unique"), bg);
  if (pi->unique)
    unique_cb->setChecked(TRUE);
  unique_cb->setDisabled(visit);
  
  bg = 
    new QButtonGroup(3, Qt::Horizontal, QString::null, htab);
  bg->setExclusive(TRUE);
  
  standard_rb = new QRadioButton(TR("standard"), bg);
  exception_rb = new QRadioButton(TR("exception"), bg);
  stream_rb = new QRadioButton(TR("stream"), bg);
  
  if (pi->exception)
    exception_rb->setChecked(TRUE);
  else if (pi->stream)
    stream_rb->setChecked(TRUE);
  else
    standard_rb->setChecked(TRUE);
  
  QVBox * vtab = new QVBox(grid);
  new QLabel(TR("description :"), vtab);
  if (! visit) {
    connect(new SmallPushButton(TR("Editor"), vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
  }
  comment = new MultiLineEdit(grid);
  comment->setReadOnly(visit);
  comment->setText(pi->browser_node->get_comment());
  comment->setFont(font);
  
  addTab(grid, "Uml");
  
  init_tab(ocltab, eduml_selection, pin->uml_selection, "Ocl",
	   SLOT(edit_uml_selection()), TRUE);

  // C++
  init_tab(cpptab, edcpp_selection, pin->cpp_selection, "C++",
	   SLOT(edit_cpp_selection()),
	   GenerationSettings::cpp_get_default_defs());

  // Java
  init_tab(javatab, edjava_selection, pin->java_selection, "Java",
	   SLOT(edit_java_selection()),
	   GenerationSettings::java_get_default_defs());
  
  // USER : list key - value
  
  grid = new QGrid(2, this);
  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(pi->browser_node, grid, visit);
  addTab(grid, TR("Properties"));
  
  //
    
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(change_tabs(QWidget *)));
  
  open_dialog(this);
}

PinDialog::~PinDialog() {
  pin->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
  
  close_dialog(this);
}

void PinDialog::change_tabs(QWidget * w) {
  if (hasOkButton()) {
    if (w == umltab)
      edname->setFocus();
    else if (w == ocltab)
      eduml_selection->setFocus();
    else if (w == cpptab)
      edcpp_selection->setFocus();
    else if (w == javatab)
      edjava_selection->setFocus();
  }
}

void PinDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

void PinDialog::init_tab(QWidget *& tab, MultiLineEdit *& ed, const char * v,
			 const char * lbl, const char * sl, bool enabled) {
  bool visit = !hasOkButton();
  QGrid * grid = new QGrid(2, this);

  tab = grid;
  grid->setMargin(5);
  grid->setSpacing(5);
  
  QVBox * vtab = new QVBox(grid);
  
  new QLabel(TR("selection : "), vtab);
  if (! visit)
    connect(new SmallPushButton(TR("Editor"), vtab), SIGNAL(clicked()), this, sl);
  ed = new MultiLineEdit(grid);

  QFont font = ed->font();

  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  ed->setFont(font);
  ed->setText(v);
  if (visit)
    ed->setReadOnly(TRUE);
  
  addTab(grid, lbl);
  
  if (! enabled)
    removePage(grid);
}

void PinDialog::menu_type() {
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
    
    m.insertItem(TR("Create class and choose it"), 2);
  }
  
  if (!visit || (index != -1) || (bn != 0)) {
    switch (m.exec(QCursor::pos())) {
    case 0:
      nodes.at(index)->select_in_browser();
      break;
    case 2:
      bn = BrowserClass::add_class(FALSE, view);
      if (bn == 0)
	return;
      bn->select_in_browser();
      // no break
    case 1:
      {
	QString s = bn->full_name(TRUE);
	
	if ((index = list.findIndex(s)) == -1) {
	  // new class, may be created through an other dialog
	  index = 0;
	  QStringList::Iterator iter = list.begin();
	  QStringList::Iterator iter_end = list.end();
	  
	  while ((iter != iter_end) && (*iter < s)) {
	    ++iter;
	    index += 1;
	  }
	  nodes.insert((unsigned) index, bn);
	  list.insert(iter, s);
	  edtype->insertItem(s, index + offset);
	}
      }
      edtype->setCurrentItem(index + offset);
      break;
    default:
      break;
    }
  }
}

#define DEF_EDIT(lang, Lang) \
void PinDialog::edit_##lang##_selection() {	\
  edit(ed##lang##_selection->text(), "object_"#lang"_selection", pin, Lang,  \
       this, (post_edit) post_edit_##lang##_##selection, edits);	\
} \
  \
void PinDialog::post_edit_##lang##_selection(PinDialog * d, QString s) \
{ \
  d->ed##lang##_selection->setText(s); \
}

DEF_EDIT(uml, TxtEdit)
DEF_EDIT(cpp, CppEdit)
DEF_EDIT(java, JavaEdit)

void PinDialog::accept() {
  if (!check_edits(edits) || !kvtable->check_unique())
    return;
    
  BrowserNode * bn = pin->browser_node;
  QString s;
  
  s = edname->text().stripWhiteSpace();
  if ((s != pin->name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlActivityPin,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical(TR("Error"), s + TR("\n\nillegal name or already used"));
  else {
    // check consistency
    UmlParamDirection dir = direction(eddir->currentText());
    bool exception = exception_rb->isChecked();
    UmlParamEffect effect = ::effect(edeffect->currentText());
    QString err;
    
    if ((dir == UmlIn) && exception)
      err = TR("An input pin cannot be an exception.\n");
    switch (effect) {
    case UmlDelete:
      if ((dir != UmlIn) && (dir != UmlInOut))
	err += TR("Only in and inout pin may have a delete effect.");
      break;
    case UmlCreate:
      if ((dir != UmlOut) && (dir != UmlInOut) && (dir != UmlReturn))
	err += TR("Only out, inout and return pin may have a create effect.");
      break;
    default:
      break;
    }
    
    if (!err.isEmpty())
      msg_critical(TR("Error"), err);
    else {
      bn->set_name(s);
      
      bool newst = pin->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
      AType t;
      
      s = edtype->currentText().stripWhiteSpace();
      if (!s.isEmpty()) {
	int index = list.findIndex(s);
	
	if (index >= 0)
	  t.type = (BrowserClass *) nodes.at(index);
	else
	  t.explicit_type = s;
      }
      pin->set_type(t);
      pin->dir = dir;
      pin->multiplicity =
	edmultiplicity->currentText().stripWhiteSpace();
      pin->ordering = ordering(edordering->currentText());
      pin->effect = effect;
      pin->is_control = is_control_cb->isChecked();
      pin->unique = unique_cb->isChecked();
      pin->in_state = edin_state->text().stripWhiteSpace();
      pin->exception = exception;
      pin->stream = stream_rb->isChecked();
      pin->uml_selection = eduml_selection->text().stripWhiteSpace();
      pin->cpp_selection = edcpp_selection->text().stripWhiteSpace();
      pin->java_selection = edjava_selection->text().stripWhiteSpace();
      
      bn->set_comment(comment->text());
      UmlWindow::update_comment_if_needed(bn);
      
      kvtable->update(bn);
      
      ProfiledStereotypes::modified(bn, newst);
      
      bn->modified();
      bn->package_modified();
      pin->modified();
      
      QTabDialog::accept();
    }
  }
}

void PinDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       pin, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void PinDialog::post_edit_description(PinDialog * d, QString s)
{
  d->comment->setText(s);
}
