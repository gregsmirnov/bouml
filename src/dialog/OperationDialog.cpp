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





#include <stdio.h>

#include <qcursor.h>
#include <qgrid.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h> 
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qradiobutton.h> 
#include <qpopupmenu.h> 
#include <qpushbutton.h> 
#include <qlayout.h>
#include <qtimer.h>

#include "OperationDialog.h"
#include "OperationData.h"
#include "ParamData.h"
#include "BrowserOperation.h"
#include "ClassData.h"
#include "ClassDialog.h"
#include "BrowserAttribute.h"
#include "AttributeData.h"
#include "BrowserRelation.h"
#include "RelationData.h"
#include "BrowserView.h"
#include "UmlDesktop.h"
#include "BrowserClass.h"
#include "KeyValueTable.h"
#include "ComboItem.h"
#include "myio.h"
#include "UmlWindow.h"
#include "DialogUtil.h"
#include "GenerationSettings.h"
#include "BrowserView.h"
#include "strutil.h"
#include "BodyDialog.h"
#include "AnnotationDialog.h"
#include "DecoratorDialog.h"
#include "Tool.h"
#include "ProfiledStereotypes.h"
#include "translate.h"

QSize OperationDialog::previous_size;

OperationDialog::OperationDialog(OperationData * o, DrawingLanguage l)
    : QTabDialog(0, 0, FALSE, WDestructiveClose), oper(o),
      cl((ClassData *) ((BrowserClass *) o->browser_node->parent())->get_data()) {
  o->browser_node->edit_start();
  
  if (o->browser_node->is_writable()) {
    setOkButton(TR("OK"));
    setCancelButton(TR("Cancel"));
  }
  else {
    setOkButton(QString::null);
    setCancelButton(TR("Close"));
  }

  setCaption(TR("Operation dialog"));

  visit = !hasOkButton();
  
  QString stereotype = cl->get_stereotype();
  QString st;
  
  st = GenerationSettings::cpp_class_stereotype(stereotype);
  cpp_undef = (st == "enum") || (st == "typedef") || (st == "ignored");
  
  st = GenerationSettings::java_class_stereotype(stereotype);
  java_undef = (st == "enum_pattern") || (st == "ignored");
  
  st = GenerationSettings::php_class_stereotype(stereotype);
  php_undef = (st == "enum") || (st == "ignored");
  
  st = GenerationSettings::python_class_stereotype(stereotype);
  python_undef = (st == "enum") || (st == "ignored");
  
  st = GenerationSettings::idl_class_stereotype(stereotype);
  idl_undef = (st == "enum") || (st == "typedef") || (st == "ignored");
  
  init_get_set();
  init_uml();
  init_cpp();
  init_java();
  init_php();
  init_python();
  init_idl();
  
  // USER : list key - value
  
  QGrid *   grid = new QGrid(2, this);

  grid->setMargin(5);
  grid->setSpacing(5);
  
  kvtable = new KeyValuesTable(o->get_browser_node(), grid, visit);
  addTab(grid, TR("Properties"));
  
  //
  
  unique = (GenerationSettings::cpp_get_default_defs())
    ? ((cpptab != 0) ? CppView : DefaultDrawingLanguage)
    : UmlView;
  if (GenerationSettings::java_get_default_defs())
    unique = ((unique == UmlView) && (javatab != 0))
      ? JavaView
      : DefaultDrawingLanguage;
  if (GenerationSettings::php_get_default_defs())
    unique = ((unique == UmlView) && (phptab != 0))
      ? PhpView
      : DefaultDrawingLanguage;
  if (GenerationSettings::python_get_default_defs())
    unique = ((unique == UmlView) && (pythontab != 0))
      ? PythonView
      : DefaultDrawingLanguage;
  if (GenerationSettings::idl_get_default_defs())
    unique = ((unique == UmlView) && (idltab != 0))
      ? IdlView
      : DefaultDrawingLanguage;
  if (unique == DefaultDrawingLanguage)
    unique = UmlView;
  
  //
  
  connect(this, SIGNAL(currentChanged(QWidget *)),
	  this, SLOT(update_all_tabs(QWidget *)));
  
  switch (l) {
  case CppView:
    if (! cpp_undef) {
      cpp_update_def();
      QTimer::singleShot(100, this, SLOT(cpp_edit_body()));
    }
    break;
  case JavaView:
    if (! java_undef) {
      java_update_def();
      QTimer::singleShot(100, this, SLOT(java_edit_body()));
    }
    break;
  case PhpView:
    if (! php_undef) {
      php_update_def();
      QTimer::singleShot(100, this, SLOT(php_edit_body()));
    }
    break;
  case PythonView:
    if (! python_undef) {
      python_update_def();
      QTimer::singleShot(100, this, SLOT(python_edit_body()));
    }
    break;
  default:
    break;
  }
  
  open_dialog(this);
}

OperationDialog::~OperationDialog() {
  oper->browser_node->edit_end();
  previous_size = size();
  
  while (!edits.isEmpty())
    edits.take(0)->close();
  
  close_dialog(this);
}

void OperationDialog::polish() {
  QTabDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}

// to manage get_ set_, search corresp attr/rel
void OperationDialog::init_get_set() {  
  get_of_attr = 0;
  set_of_attr = 0;
  get_of_rel = 0;
  set_of_rel = 0;
  
  if (oper->is_get_or_set) {
    BrowserOperation * br_op = (BrowserOperation *) oper->browser_node;
    QListViewItem * child;
    
    for (child = br_op->parent()->firstChild();
	 child != 0;
	 child = child->nextSibling()) {
      BrowserNode * br = (BrowserNode *) child;
      
      if (!br->deletedp()) {
	if (br->get_type() == UmlAttribute) {
	  if (((BrowserAttribute *) br)->get_get_oper() == br_op) {
	    get_of_attr = (AttributeData *) br->get_data();
	    break;
	  }
	  if (((BrowserAttribute *) br)->get_set_oper() == br_op) {
	    set_of_attr = (AttributeData *) br->get_data();
	    break;
	  }
	}
	else if (IsaRelation(br->get_type())) {
	  if (((BrowserRelation *) br)->get_get_oper() == br_op) {
	    get_of_rel = (RelationData *) br->get_data();
	    is_rel_a = get_of_rel->is_a((BrowserRelation *) br);
	    break;
	  }
	  if (((BrowserRelation *) br)->get_set_oper() == br_op) {
	    set_of_rel = (RelationData *) br->get_data();
	    is_rel_a = set_of_rel->is_a((BrowserRelation *) br);
	    break;
	  }
	}
      }
    }
  }
}

// general tab
void OperationDialog::init_uml() {  
  QGrid * grid;
  QHBox * htab;
  QButtonGroup * bg;
    
  grid = new QGrid(2, this);
  umltab = grid;
  grid->setSpacing(5);
  grid->setMargin(5);
  
  new QLabel(TR("class : "), grid);
  new QLabel(((BrowserNode *) oper->get_browser_node()->parent())->full_name(TRUE),
	     grid);
  
  new QLabel(TR("name : "), grid);
  edname = new LineEdit(oper->name(), grid);
  edname->setReadOnly(visit);
  
  new QLabel(TR("stereotype : "), grid);
  edstereotype = new QComboBox(!visit, grid);
  edstereotype->insertItem(toUnicode(oper->stereotype));
  if (oper->is_get_or_set)
    edstereotype->setEnabled(FALSE);
  else if (! visit) {
    edstereotype->insertStringList(BrowserOperation::default_stereotypes());
    edstereotype->insertStringList(ProfiledStereotypes::defaults(UmlOperation));
    edstereotype->setAutoCompletion(completion());
  }
  edstereotype->setCurrentItem(0);
  
  QSizePolicy sp = edstereotype->sizePolicy();
  
  sp.setHorData(QSizePolicy::Expanding);
  edstereotype->setSizePolicy(sp);

  if (oper->is_get_or_set)
    new QLabel(TR("value type : "), grid);
  else
    connect(new SmallPushButton(TR("value type :"), grid), SIGNAL(clicked()),
	    this, SLOT(menu_returntype()));
  
  BrowserClass::instances(nodes);
  nodes.full_names(list);

  edreturn_type = new QComboBox(!visit, grid);
  edreturn_type->insertItem(oper->get_return_type().get_full_type());
  if (oper->is_get_or_set)
    edreturn_type->setEnabled(FALSE);
  else if (! visit) {
    QStringList l = GenerationSettings::basic_types();
    
    cl->addFormals(l);
    edreturn_type->insertStringList(l);
    edreturn_type_offset = edreturn_type->count();
    edreturn_type->insertStringList(list);
    edreturn_type->setAutoCompletion(completion());
    view = oper->browser_node->container(UmlClass);
  }
  edreturn_type->setCurrentItem(0);
  
  edreturn_type->setSizePolicy(sp);

  new QLabel(grid);
  
  htab = new QHBox(grid);
  bg = uml_visibility.init(htab, oper->get_uml_visibility(), TRUE);
  if (visit)
    bg->setEnabled(FALSE);
  htab->setStretchFactor(bg, 1000);
  
  htab->setStretchFactor(new QLabel("  ", htab), 0);
  
  bg = new QButtonGroup(2, Qt::Horizontal, QString::null, htab);
  htab->setStretchFactor(bg, 1000);
  classoperation_cb = new QCheckBox(TR("static"), bg);
  classoperation_cb->setDisabled(visit);
  abstract_cb = new QCheckBox(TR("abstract"), bg);
  abstract_cb->setDisabled(visit);
  if (oper->get_isa_class_operation())
    classoperation_cb->setChecked(TRUE);
  else if (oper->get_is_abstract())
    abstract_cb->setChecked(TRUE);
  if (oper->is_get_or_set)
    bg->setEnabled(FALSE);
  else if (! visit) {
    connect(classoperation_cb, SIGNAL(toggled(bool)),
	    SLOT(classoper_toggled(bool)));
    connect(abstract_cb, SIGNAL(toggled(bool)),
	    SLOT(abstract_toggled(bool)));
  }
  
  htab->setStretchFactor(new QLabel("  ", htab), 0);
  
  bg = new QButtonGroup(1, Qt::Horizontal, QString::null, htab);
  htab->setStretchFactor(bg, 1000);
  forcegenbody_cb = new QCheckBox(TR("force body generation"), bg);
  forcegenbody_cb->setDisabled(visit);
  if (oper->body_generation_forced())
    forcegenbody_cb->setChecked(TRUE);
  else if (! visit)
    connect(forcegenbody_cb, SIGNAL(toggled(bool)),
	    SLOT(forcegenbody_toggled(bool)));
  
  new QLabel(TR("parameters : "), grid);
  table = new ParamsTable(oper, grid, list, this, visit);
  if (oper->is_get_or_set)
    table->setEnabled(FALSE);
  
  new QLabel(TR("exceptions : "), grid);
  etable = new ExceptionsTable(oper, grid, list, visit);
  
  QVBox * vtab = new QVBox(grid);
  
  new QLabel(TR("description :"), vtab);
  if (! visit) {
    connect(new SmallPushButton(TR("Editor"), vtab), SIGNAL(clicked()),
	    this, SLOT(edit_description()));
    connect(new SmallPushButton(TR("Default"), vtab), SIGNAL(clicked()),
	    this, SLOT(default_description()));
  }
  comment = new MultiLineEdit(grid);
  comment->setText(oper->get_browser_node()->get_comment());
  QFont font = comment->font();
  if (! hasCodec())
    font.setFamily("Courier");
  font.setFixedPitch(TRUE);
  comment->setFont(font);
  comment->setReadOnly(visit);
  
  vtab = new QVBox(grid);
  new QLabel(TR("constraint :"), vtab);
  if (! visit) {
    connect(new SmallPushButton(TR("Editor"), vtab), SIGNAL(clicked()),
	    this, SLOT(edit_constraint()));
  }
  constraint = new MultiLineEdit(grid);
  constraint->setReadOnly(visit);
  constraint->setText(oper->constraint);
  constraint->setFont(font);
  
  addTab(grid, "Uml");
}

  
// C++
void OperationDialog::init_cpp() {  
  if (! cpp_undef) {
    QGrid * grid;
    QHBox * htab;
    QButtonGroup * bg;
    
    grid = new QGrid(2, this);
    cpptab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);

    if (visit || !oper->is_get_or_set)
      new QLabel(grid);
    else {
      cppfrozen_cb = new QCheckBox(TR("frozen"), grid);
      if (oper->cpp_get_set_frozen)
	cppfrozen_cb->setChecked(TRUE);
    }

    htab = new QHBox(grid);
    
    bg = cpp_visibility.init(htab, oper->get_cpp_visibility(), FALSE, 0, TR("follow uml"));
    if (visit)
      bg->setEnabled(FALSE);
    
    htab->setStretchFactor(new QLabel("      ", htab), 0);
    
    bg = new QButtonGroup(5, Qt::Horizontal, QString::null, htab);
    const_cb = new QCheckBox("const", bg);
    volatile_cb = new QCheckBox("volatile", bg);
    friend_cb = new QCheckBox("friend", bg);
    virtual_cb = new QCheckBox("virtual", bg);
    inline_cb = new QCheckBox("inline", bg);
    if (oper->get_cpp_const())
      const_cb->setChecked(TRUE);
    if (oper->get_is_volatile())
      volatile_cb->setChecked(TRUE);
    if (oper->get_cpp_friend())
      friend_cb->setChecked(TRUE);
    if (oper->get_cpp_virtual())
      virtual_cb->setChecked(TRUE);
    if (oper->get_cpp_inline())
      inline_cb->setChecked(TRUE);
    if (visit) {
      const_cb->setDisabled(TRUE);
      volatile_cb->setDisabled(TRUE);
      virtual_cb->setDisabled(TRUE);
      friend_cb->setDisabled(TRUE);
      inline_cb->setDisabled(TRUE);
    }
    else {
      connect(const_cb, SIGNAL(toggled(bool)),
	      SLOT(const_volatile_toggled(bool)));
      connect(volatile_cb, SIGNAL(toggled(bool)),
	      SLOT(const_volatile_toggled(bool)));
      connect(friend_cb, SIGNAL(toggled(bool)),
	      SLOT(friend_toggled(bool)));
      connect(virtual_cb, SIGNAL(toggled(bool)),
	      SLOT(virtual_toggled(bool)));
      connect(inline_cb, SIGNAL(toggled(bool)),
	      SLOT(inline_toggled(bool)));
    }
    
    if (oper->is_get_or_set) {
      new QLabel(TR("Name form : "), grid);
      edcppnamespec = new LineEdit(grid);
      edcppnamespec->setText(oper->cpp_name_spec);
      if (visit)
	edcppnamespec->setReadOnly(TRUE);
      else {
	connect(edcppnamespec, SIGNAL(textChanged(const QString &)), this, SLOT(cpp_update_decl()));
	connect(edcppnamespec, SIGNAL(textChanged(const QString &)), this, SLOT(cpp_update_def()));
      }
    }
    else
      edcppnamespec = 0;
    
    new QLabel(TR("Declaration : "), grid);
    edcppdecl = new MultiLineEdit(grid);
    edcppdecl->setText(oper->get_cppdecl());
    edcppdecl->setFont(comment->font());
    if (visit)
      edcppdecl->setReadOnly(TRUE);
    else
      connect(edcppdecl, SIGNAL(textChanged()), this, SLOT(cpp_update_decl()));
    
    new QLabel(TR("Result after\nsubstitution : "), grid);
    showcppdecl = new MultiLineEdit(grid);
    showcppdecl->setReadOnly(TRUE);
    showcppdecl->setFont(comment->font());
    
    if (! visit) {
      new QLabel(grid);
      htab = new QHBox(grid);  
      connect(new QPushButton(TR("Default declaration"), htab), SIGNAL(clicked()),
	      this, SLOT(cpp_default_decl()));
      if (!oper->is_get_or_set)
	connect(new QPushButton(TR("From definition"), htab), SIGNAL(clicked()),
		this, SLOT(cpp_decl_from_def()));
      connect(new QPushButton(TR("Not generated in C++"), htab), SIGNAL(clicked()),
	      this, SLOT(cpp_unmapped_decl()));
      connect(new QPushButton(TR("Edit parameters"), htab), SIGNAL(clicked()),
	      this, SLOT(cpp_edit_param_decl()));
    }
    
    new QLabel(TR("Definition :"), grid);
    edcppdef = new MultiLineEdit(grid);
    edcppdef->setText(oper->get_cppdef());
    edcppdef->setFont(comment->font());
    if (visit)
      edcppdef->setReadOnly(TRUE);
    else
      connect(edcppdef, SIGNAL(textChanged()), this, SLOT(cpp_update_def()));
    
    QVBox * vtab = new QVBox(grid);

    new QLabel(TR("Result after\nsubstitution : "), vtab);
    if (!visit && !oper->is_get_or_set) {
      indentcppbody_cb = new QCheckBox(TR("contextual\nbody indent"), vtab);
      if (preserve_bodies() && !forcegenbody_cb->isChecked())
	indentcppbody_cb->setEnabled(FALSE);
      else
	indentcppbody_cb->setChecked(oper->cpp_indent_body);
    }
    
    showcppdef = new MultiLineEdit(grid);
    showcppdef->setReadOnly(TRUE);
    showcppdef->setFont(comment->font());
    
    editcppbody = new QPushButton((visit ||
				   (preserve_bodies() && !forcegenbody_cb->isChecked()))
				  ? TR("Show body") : TR("Edit body"),
				  grid);
    connect(editcppbody, SIGNAL(clicked()), this, SLOT(cpp_edit_body()));
    
    char * b = oper->get_body('c');
    
    if (b != 0) {
      cppbody = oldcppbody = b;
      delete [] b;
    }
        
    if (! visit) {
      htab = new QHBox(grid);  
      connect(new QPushButton(TR("Default definition"), htab), SIGNAL(clicked ()),
	      this, SLOT(cpp_default_def()));
      if (!oper->is_get_or_set)
	connect(new QPushButton(TR("From declaration"), htab), SIGNAL(clicked ()),
		this, SLOT(cpp_def_from_decl()));
      connect(new QPushButton(TR("Not generated in C++"), htab), SIGNAL(clicked ()),
	      this, SLOT(cpp_unmapped_def()));
      connect(new QPushButton(TR("Edit parameters"), htab), SIGNAL(clicked()),
	      this, SLOT(cpp_edit_param_def()));
    }
    
    addTab(grid, "C++");
  
    if (!GenerationSettings::cpp_get_default_defs())
      removePage(grid);
    
    cl->get_class_spec(templates, cl_names, templates_tmplop, cl_names_tmplop);
    
    if (!templates.isEmpty())
      inline_cb->setDisabled(TRUE);
  }
  else
    cpptab = 0;
}
  
// Java
void OperationDialog::init_java() {  
  if (! java_undef) {
    QGrid * grid;
    QHBox * htab;
    QButtonGroup * bg;

    grid = new QGrid(2, this);
    javatab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (visit || !oper->is_get_or_set)
      new QLabel(grid);
    else {
      javafrozen_cb = new QCheckBox(TR("frozen"), grid);
      if (oper->java_get_set_frozen)
	javafrozen_cb->setChecked(TRUE);
    }

    bg = new QButtonGroup(2, Qt::Horizontal, QString::null, grid);
    javafinal_cb = new QCheckBox("final", bg);
    if (oper->get_java_final())
      javafinal_cb->setChecked(TRUE);
    if (visit)
      javafinal_cb->setDisabled(TRUE);
    else
      connect(javafinal_cb, SIGNAL(toggled(bool)),
	      SLOT(java_finalsynchronized_toggled(bool)));

    synchronized_cb = new QCheckBox("synchronized", bg);
    if (oper->get_java_synchronized())
      synchronized_cb->setChecked(TRUE);
    if (visit)
      synchronized_cb->setDisabled(TRUE);
    else
      connect(synchronized_cb, SIGNAL(toggled(bool)),
	      SLOT(java_finalsynchronized_toggled(bool)));
    
    if (oper->is_get_or_set) {
      new QLabel(TR("Name form : "), grid);
      htab = new QHBox(grid);
      edjavanamespec = new LineEdit(htab);
      edjavanamespec->setText(oper->java_name_spec);
      if (visit)
	edjavanamespec->setDisabled(TRUE);
      else {
	connect(edjavanamespec, SIGNAL(textChanged(const QString &)), this, SLOT(java_update_def()));
      }
    }
    else
      edjavanamespec = 0;
    
    new QLabel(TR("Definition :"), grid);
    edjavadef = new MultiLineEdit(grid);
    edjavadef->setText(oper->get_javadef());
    edjavadef->setFont(comment->font());
    if (visit)
      edjavadef->setReadOnly(TRUE);
    else
      connect(edjavadef, SIGNAL(textChanged()), this, SLOT(java_update_def()));
    
    QVBox * vtab = new QVBox(grid);

    new QLabel(TR("Result after\nsubstitution : "), vtab);
    if (!visit && !oper->is_get_or_set) {
      indentjavabody_cb = new QCheckBox(TR("contextual\nbody indent"), vtab);
      if (preserve_bodies() && !forcegenbody_cb->isChecked())
	indentjavabody_cb->setEnabled(FALSE);
      else
	indentjavabody_cb->setChecked(oper->java_indent_body);
    }
    
    showjavadef = new MultiLineEdit(grid);
    showjavadef->setReadOnly(TRUE);
    showjavadef->setFont(comment->font());
    
    editjavabody = new QPushButton((visit ||
				    (preserve_bodies() && !forcegenbody_cb->isChecked()))
				   ? TR("Show body") : TR("Edit body"),
				   grid);
    connect(editjavabody, SIGNAL(clicked()), this, SLOT(java_edit_body()));
    
    char * b = oper->get_body('j');
    
    if (b != 0) {
      javabody = oldjavabody = b;
      delete [] b;
    }
        
    htab = new QHBox(grid);  

    if (! visit) {
      connect(new QPushButton(TR("Default definition"), htab), SIGNAL(clicked ()),
	      this, SLOT(java_default_def()));
      connect(new QPushButton(TR("Not generated in Java"), htab), SIGNAL(clicked ()),
	      this, SLOT(java_unmapped_def()));
      
    }
    
    javaannotation = (const char *) oper->java_annotation;
    editjavaannotation =
      new QPushButton((visit) ? TR("Show annotation") : TR("Edit annotation"),
		      htab);
    connect(editjavaannotation, SIGNAL(clicked ()),
	    this, SLOT(java_edit_annotation()));
    
    addTab(grid, "Java");
  
    if (!GenerationSettings::java_get_default_defs())
      removePage(grid);
  }
  else
    javatab = 0;
}
    
// Php
void OperationDialog::init_php() {  
  if (! php_undef) {
    QGrid * grid;
    QHBox * htab;
    QButtonGroup * bg;

    grid = new QGrid(2, this);
    phptab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (visit || !oper->is_get_or_set)
      new QLabel(grid);
    else {
      phpfrozen_cb = new QCheckBox(TR("frozen"), grid);
      if (oper->php_get_set_frozen)
	phpfrozen_cb->setChecked(TRUE);
    }

    bg = new QButtonGroup(2, Qt::Horizontal, QString::null, grid);
    phpfinal_cb = new QCheckBox("final", bg);
    if (oper->get_php_final())
      phpfinal_cb->setChecked(TRUE);
    if (visit)
      phpfinal_cb->setDisabled(TRUE);
    else
      connect(phpfinal_cb, SIGNAL(toggled(bool)),
	      SLOT(php_final_toggled(bool)));

    if (oper->is_get_or_set) {
      new QLabel(TR("Name form : "), grid);
      htab = new QHBox(grid);
      edphpnamespec = new LineEdit(htab);
      edphpnamespec->setText(oper->php_name_spec);
      if (visit)
	edphpnamespec->setDisabled(TRUE);
      else {
	connect(edphpnamespec, SIGNAL(textChanged(const QString &)), this, SLOT(php_update_def()));
      }
    }
    else
      edphpnamespec = 0;
    
    new QLabel(TR("Definition :"), grid);
    edphpdef = new MultiLineEdit(grid);
    edphpdef->setText(oper->get_phpdef());
    edphpdef->setFont(comment->font());
    if (visit)
      edphpdef->setReadOnly(TRUE);
    else
      connect(edphpdef, SIGNAL(textChanged()), this, SLOT(php_update_def()));
    
    QVBox * vtab = new QVBox(grid);

    new QLabel(TR("Result after\nsubstitution : "), vtab);
    if (!visit && !oper->is_get_or_set) {
      indentphpbody_cb = new QCheckBox(TR("contextual\nbody indent"), vtab);
      if (preserve_bodies() && !forcegenbody_cb->isChecked())
	indentphpbody_cb->setEnabled(FALSE);
      else
	indentphpbody_cb->setChecked(oper->php_indent_body);
    }
    
    showphpdef = new MultiLineEdit(grid);
    showphpdef->setReadOnly(TRUE);
    showphpdef->setFont(comment->font());
    
    editphpbody = new QPushButton((visit ||
				   (preserve_bodies() && !forcegenbody_cb->isChecked()))
				  ? TR("Show body") : TR("Edit body"),
				  grid);
    connect(editphpbody, SIGNAL(clicked()), this, SLOT(php_edit_body()));
    
    char * b = oper->get_body('p');
    
    if (b != 0) {
      phpbody = oldphpbody = b;
      delete [] b;
    }
        
    htab = new QHBox(grid);  

    if (! visit) {
      connect(new QPushButton(TR("Default definition"), htab), SIGNAL(clicked ()),
	      this, SLOT(php_default_def()));
      connect(new QPushButton(TR("Not generated in Php"), htab), SIGNAL(clicked ()),
	      this, SLOT(php_unmapped_def()));
      connect(new QPushButton(TR("Edit parameters"), htab), SIGNAL(clicked()),
	      this, SLOT(php_edit_param()));      
    }
    
    addTab(grid, "Php");
  
    if (!GenerationSettings::php_get_default_defs())
      removePage(grid);
  }
  else
    phptab = 0;
}

// Python
void OperationDialog::init_python() {  
  if (! python_undef) {
    QGrid * grid;
    QHBox * htab;

    grid = new QGrid(2, this);
    pythontab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (!visit && oper->is_get_or_set) {
      pythonfrozen_cb = new QCheckBox(TR("frozen"), grid);
      if (oper->python_get_set_frozen)
	pythonfrozen_cb->setChecked(TRUE);
      new QLabel(grid);
    }

    if (oper->is_get_or_set) {
      new QLabel(TR("Name form : "), grid);
      htab = new QHBox(grid);
      edpythonnamespec = new LineEdit(htab);
      edpythonnamespec->setText(oper->python_name_spec);
      if (visit)
	edpythonnamespec->setDisabled(TRUE);
      else {
	connect(edpythonnamespec, SIGNAL(textChanged(const QString &)), this, SLOT(python_update_def()));
      }
    }
    else
      edpythonnamespec = 0;
    
    new QLabel(TR("Definition :"), grid);
    edpythondef = new MultiLineEdit(grid);
    edpythondef->setText(oper->get_pythondef());
    edpythondef->setFont(comment->font());
    if (visit)
      edpythondef->setReadOnly(TRUE);
    else
      connect(edpythondef, SIGNAL(textChanged()), this, SLOT(python_update_def()));
    
    QVBox * vtab = new QVBox(grid);

    new QLabel(TR("Result after\nsubstitution : "), vtab);
    if (!visit && !oper->is_get_or_set) {
      indentpythonbody_cb = new QCheckBox(TR("contextual\nbody indent"), vtab);
      if (preserve_bodies() && !forcegenbody_cb->isChecked())
	indentpythonbody_cb->setEnabled(FALSE);
      else
	indentpythonbody_cb->setChecked(oper->python_indent_body);
    }
    showpythondef = new MultiLineEdit(grid);
    showpythondef->setReadOnly(TRUE);
    showpythondef->setFont(comment->font());
    
    editpythonbody = new QPushButton((visit ||
				   (preserve_bodies() && !forcegenbody_cb->isChecked()))
				  ? TR("Show body") : TR("Edit body"),
				  grid);
    connect(editpythonbody, SIGNAL(clicked()), this, SLOT(python_edit_body()));
    
    char * b = oper->get_body('y');
    
    if (b != 0) {
      pythonbody = oldpythonbody = b;
      delete [] b;
    }
        
    htab = new QHBox(grid);  

    if (! visit) {
      connect(new QPushButton(TR("Default definition"), htab), SIGNAL(clicked ()),
	      this, SLOT(python_default_def()));
      connect(new QPushButton(TR("Not generated in Python"), htab), SIGNAL(clicked ()),
	      this, SLOT(python_unmapped_def()));
      connect(new QPushButton(TR("Edit parameters"), htab), SIGNAL(clicked()),
	      this, SLOT(python_edit_param()));      
    }
    
    pythondecorator = (const char *) oper->python_decorator;
    editpythondecorator =
      new QPushButton((visit) ? TR("Show decorators") : TR("Edit decorators"),
		      htab);
    connect(editpythondecorator, SIGNAL(clicked ()),
	    this, SLOT(python_edit_decorator()));

    addTab(grid, "Python");
  
    if (!GenerationSettings::python_get_default_defs())
      removePage(grid);
  }
  else
    pythontab = 0;
}

// IDL
void OperationDialog::init_idl() {  
  if (! idl_undef) {
    QGrid * grid;
    QHBox * htab;
    QButtonGroup * bg;

    grid = new QGrid(2, this);
    idltab = grid;
    grid->setMargin(5);
    grid->setSpacing(5);
    
    if (visit || !oper->is_get_or_set)
      new QLabel(grid);
    else {
      idlfrozen_cb = new QCheckBox(TR("frozen"), grid);
      if (oper->idl_get_set_frozen)
	idlfrozen_cb->setChecked(TRUE);
    }

    bg = new QButtonGroup(1, Qt::Horizontal, QString::null, grid);
    oneway_cb = new QCheckBox("oneway", bg);
    if (oper->get_idl_oneway())
      oneway_cb->setChecked(TRUE);
    if (visit)
      oneway_cb->setDisabled(TRUE);
    else
      connect(oneway_cb, SIGNAL(toggled(bool)),
	      SLOT(oneway_toggled(bool)));
    
    if (oper->is_get_or_set) {
      new QLabel(TR("Name form : "), grid);
      htab = new QHBox(grid);
      edidlnamespec = new LineEdit(htab);
      edidlnamespec->setText(oper->idl_name_spec);
      if (visit)
	edidlnamespec->setReadOnly(TRUE);
      else {
	connect(edidlnamespec, SIGNAL(textChanged(const QString &)), this, SLOT(idl_update_decl()));
      }
    }
    else
      edidlnamespec = 0;
    
    new QLabel(TR("Declaration :"), grid);
    edidldecl = new MultiLineEdit(grid);
    edidldecl->setText(oper->get_idldecl());
    edidldecl->setFont(comment->font());
    if (visit)
      edidldecl->setReadOnly(TRUE);
    else
      connect(edidldecl, SIGNAL(textChanged()), this, SLOT(idl_update_decl()));
    
    new QLabel(TR("Result after\nsubstitution : "), grid);
    showidldecl = new MultiLineEdit(grid);
    showidldecl->setReadOnly(TRUE);
    showidldecl->setFont(comment->font());
    
    if (! visit) {
      new QLabel(grid);
      htab = new QHBox(grid);  
      connect(new QPushButton(TR("Default declaration"), htab), SIGNAL(clicked ()),
	      this, SLOT(idl_default_def()));
      connect(new QPushButton(TR("Not generated in Idl"), htab), SIGNAL(clicked ()),
	      this, SLOT(idl_unmapped_def()));
    }
    
    addTab(grid, "Idl");
  
    if (!GenerationSettings::idl_get_default_defs())
      removePage(grid);
  }
  else
    idltab = 0;
}

void OperationDialog::menu_returntype() {
  QPopupMenu m(0);

  m.insertItem(TR("Choose"), -1);
  m.insertSeparator();
  
  int index = list.findIndex(edreturn_type->currentText().stripWhiteSpace());
  
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
	  edreturn_type->insertItem(s, index + edreturn_type_offset);
	}
      }
      edreturn_type->setCurrentItem(index + edreturn_type_offset);
    }
  }
}

void OperationDialog::default_description() {
  comment->setText(GenerationSettings::default_operation_description());
}

void OperationDialog::edit_description() {
  edit(comment->text(), edname->text().stripWhiteSpace() + "_description",
       oper, TxtEdit, this, (post_edit) post_edit_description, edits);
}

void OperationDialog::post_edit_description(OperationDialog * d, QString s)
{
  d->comment->setText(s);
}

void OperationDialog::edit_constraint() {
  edit(constraint->text(), edname->text().stripWhiteSpace() + "_constraint",
       oper, TxtEdit, this, (post_edit) post_edit_constraint, edits);
}

void OperationDialog::post_edit_constraint(OperationDialog * d, QString s)
{
  d->constraint->setText(s);
}

void OperationDialog::accept() {
  if (!check_edits(edits) || !kvtable->check_unique())
    return;
    
  BrowserNode * bn = oper->browser_node;
  QString s = edname->text().stripWhiteSpace();
  
  if ((s != oper->name()) &&
      ((BrowserNode *) bn->parent())->wrong_child_name(s, UmlOperation,
						       bn->allow_spaces(),
						       bn->allow_empty()))
    msg_critical(TR("Error"), s + TR("\n\nillegal name or already used"));
  else {  
    bn->set_name(s);
    
    bool newst = FALSE;
    
    if (!oper->is_get_or_set) {
      AType t;
      
      s = edreturn_type->currentText().stripWhiteSpace();
      
      if (! s.isEmpty()) {
	int index = list.findIndex(edreturn_type->currentText());
	
	if (index >= 0)
	  t.type = (BrowserClass *) nodes.at(index);
	else
	  t.explicit_type = s;
      }
      oper->set_return_type(t);
      
      newst = oper->set_stereotype(fromUnicode(edstereotype->currentText().stripWhiteSpace()));
    }
    
    oper->uml_visibility = uml_visibility.value();
    
    oper->isa_class_operation = classoperation_cb->isChecked();
    oper->set_is_abstract(abstract_cb->isChecked());
    
    oper->force_body_gen = forcegenbody_cb->isChecked();
    
    table->update(oper, nodes);
    etable->update(oper, nodes);
    
    bn->set_comment(comment->text());
    UmlWindow::update_comment_if_needed(bn);
  
    oper->constraint = constraint->stripWhiteSpaceText();
    
    // C++
    
    if (cpp_undef) {
      oper->cpp_decl = QString::null;
      oper->cpp_def.assign(QString::null, TRUE);
      if (!oldcppbody.isEmpty())
	oper->new_body(QString::null, 'c');
    }
    else {
      if (oper->is_get_or_set) {
	oper->cpp_name_spec = edcppnamespec->text().stripWhiteSpace();
	oper->cpp_body.length = 0;
	oper->cpp_get_set_frozen = cppfrozen_cb->isChecked();
      }
      else
	oper->cpp_indent_body = indentcppbody_cb->isChecked();
      if (!abstract_cb->isChecked() && 
	  (edcppdef->text().find("${body}") != -1)) {
	if (cppbody != oldcppbody)
	  oper->new_body(cppbody, 'c');
      }
      else if(!oldcppbody.isEmpty())
	oper->new_body(QString::null, 'c');
      oper->cpp_decl = edcppdecl->text();
      oper->cpp_visibility = cpp_visibility.value();
      
      oper->cpp_const = const_cb->isChecked();
      oper->is_volatile = volatile_cb->isChecked();
      oper->cpp_friend = friend_cb->isChecked();
      oper->cpp_virtual = virtual_cb->isChecked();
      oper->cpp_inline = inline_cb->isChecked();
      oper->cpp_def.assign(edcppdef->text(),
			   abstract_cb->isChecked() ||
			   (edcppdef->text().find("${body}") != -1));
    }
    
    // java
    
    if (java_undef) {
      oper->java_def.assign(QString::null, TRUE);
      if (!oldjavabody.isEmpty())
	oper->new_body(QString::null, 'j');
    }
    else {
      if (oper->is_get_or_set) {
	oper->java_name_spec = edjavanamespec->text().stripWhiteSpace();
	oper->java_get_set_frozen = javafrozen_cb->isChecked();
      }
      else
	oper->java_indent_body = indentjavabody_cb->isChecked();
      
      QString ste = GenerationSettings::java_class_stereotype(cl->get_stereotype());
      bool interf = (ste == "interface") || (ste == "@interface");
      
      if (!abstract_cb->isChecked() && !interf &&
	  (edjavadef->text().find("${body}") != -1)) {
	if (javabody != oldjavabody)
	  oper->new_body(javabody, 'j');
      }
      else if (!oldjavabody.isEmpty())
	oper->new_body(QString::null, 'j');
      oper->java_final = javafinal_cb->isChecked();
      oper->java_synchronized = synchronized_cb->isChecked();
      oper->java_def.assign(edjavadef->text(),
			    abstract_cb->isChecked() || interf ||
			    (edjavadef->text().find("${body}") != -1));
      
      oper->java_annotation = javaannotation;
    }
    
    // php
    
    if (php_undef) {
      oper->php_def.assign(QString::null, TRUE);
      if (!oldphpbody.isEmpty())
	oper->new_body(QString::null, 'p');
    }
    else {
      if (oper->is_get_or_set) {
	oper->php_name_spec = edphpnamespec->text().stripWhiteSpace();
	oper->php_get_set_frozen = phpfrozen_cb->isChecked();
      }
      else
	oper->php_indent_body = indentphpbody_cb->isChecked();
      
      QString ste = GenerationSettings::php_class_stereotype(cl->get_stereotype());
      bool interf = (ste == "interface");
      
      if (!abstract_cb->isChecked() && !interf &&
	  (edphpdef->text().find("${body}") != -1)) {
	if (phpbody != oldphpbody)
	  oper->new_body(phpbody, 'p');
      }
      else if (!oldphpbody.isEmpty())
	oper->new_body(QString::null, 'p');
      oper->php_final = phpfinal_cb->isChecked();
      oper->php_def.assign(edphpdef->text(),
			    abstract_cb->isChecked() || interf ||
			    (edphpdef->text().find("${body}") != -1));
    }
    
    // python
    
    if (python_undef) {
      oper->python_def.assign(QString::null, TRUE);
      if (!oldpythonbody.isEmpty())
	oper->new_body(QString::null, 'y');
    }
    else {
      if (oper->is_get_or_set) {
	oper->python_name_spec = edpythonnamespec->text().stripWhiteSpace();
	oper->python_get_set_frozen = pythonfrozen_cb->isChecked();
      }
      else
	oper->python_indent_body = indentpythonbody_cb->isChecked();
            
      // rmq : abstractmethod have body !
      if (edpythondef->text().find("${body}") != -1) {
	if (pythonbody != oldpythonbody)
	  oper->new_body(pythonbody, 'y');
      }
      else if (!oldpythonbody.isEmpty())
	oper->new_body(QString::null, 'y');
      oper->python_def.assign(edpythondef->text(),
			      // rmq : abstractmethod have body !
			      (edpythondef->text().find("${body}") != -1));
      
      oper->python_decorator = pythondecorator;
    }
    
    // idl
    
    if (idl_undef)
      oper->idl_decl = QString::null;
    else {
      if (oper->is_get_or_set) {
	oper->idl_name_spec = edidlnamespec->text().stripWhiteSpace();
	oper->idl_get_set_frozen = idlfrozen_cb->isChecked();
      }
      oper->idl_oneway = oneway_cb->isChecked();
      oper->idl_decl = edidldecl->text();
    }
    
    // user
    
    kvtable->update(bn);
    
    ProfiledStereotypes::modified(bn, newst);
    
    bn->modified();
    bn->package_modified();
    oper->modified();
    
    QTabDialog::accept();
  }
}

void OperationDialog::classoper_toggled(bool on) {
  if (on) {
    abstract_cb->setChecked(FALSE);
    if (! cpp_undef)
      virtual_cb->setChecked(FALSE);
  }
}

void OperationDialog::abstract_toggled(bool on) {
  if (on) {
    classoperation_cb->setChecked(FALSE);
    if (! cpp_undef)
      virtual_cb->setChecked(TRUE);
  }
}

void OperationDialog::forcegenbody_toggled(bool on) {
  bool ro = (visit || (preserve_bodies() && !on));
  QString lbl = (ro) ? TR("Show body") : TR("Edit body");

  if (!cpp_undef)
    editcppbody->setText(lbl);
  if (!java_undef)
    editjavabody->setText(lbl);
  if (!php_undef)
    editphpbody->setText(lbl);
  if (!python_undef)
    editpythonbody->setText(lbl);
  
  if (!visit && !oper->is_get_or_set) {
    if (!cpp_undef)
      indentcppbody_cb->setEnabled(editcppbody->isEnabled() && !ro);
    if (!java_undef)
      indentjavabody_cb->setEnabled(editjavabody->isEnabled() && !ro);
    if (!php_undef)
      indentphpbody_cb->setEnabled(editphpbody->isEnabled() && !ro);
    if (!python_undef)
      indentpythonbody_cb->setEnabled(editpythonbody->isEnabled() && !ro);
  }
}

void OperationDialog::update_all_tabs(QWidget * w) {
  table->forceUpdateCells();
  etable->forceUpdateCells();
  
  if (w == umltab) {
    if (! visit)
      edname->setFocus();
  }
  else if (w == cpptab) {
    cpp_update_decl();
    cpp_update_def();
    if (! visit)
      edcppdecl->setFocus();
  }
  else if (w == javatab) {
    java_update_def();
    if (! visit)
      edjavadef->setFocus();
  }
  else if (w == phptab) {
    php_update_def();
    if (! visit)
      edphpdef->setFocus();
  }
  else if (w == pythontab) {
    python_update_def();
    if (! visit)
      edpythondef->setFocus();
  }
  else if (w == idltab) {
    idl_update_decl();
    if (! visit)
      edidldecl->setFocus();
  }
}

// C++

void OperationDialog::const_volatile_toggled(bool) {
  cpp_update_decl();
  cpp_update_def();
}

void OperationDialog::virtual_toggled(bool on) {
  if (on)
    friend_cb->setChecked(FALSE);
  cpp_update_decl();
  cpp_update_def();
}

void OperationDialog::friend_toggled(bool on) {
  if (on) {
    inline_cb->setChecked(FALSE);
    virtual_cb->setChecked(FALSE);
  }
  cpp_update_decl();
  cpp_update_def();
}

void OperationDialog::inline_toggled(bool on) {
  if (on)
    friend_cb->setChecked(FALSE);
  cpp_update_decl();
  cpp_update_def();
}

void OperationDialog::cpp_default_decl() {
  if (oper->is_get_or_set) {
    QCString decl;
    QCString def;
    
    if (get_of_attr != 0)
      oper->update_cpp_get_of(decl, def, get_of_attr->get_browser_node()->get_name(),
			      get_of_attr->get_cppdecl(),
			      get_of_attr->get_isa_const_attribute(),
			      get_of_attr->get_multiplicity());
    else if (set_of_attr != 0)
      oper->update_cpp_set_of(decl, def, set_of_attr->get_browser_node()->get_name(),
			      set_of_attr->get_cppdecl(),
			      set_of_attr->get_isa_const_attribute(),
			      set_of_attr->get_multiplicity());
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_cpp_get_of(decl, def, get_of_rel->get_role_a(), 
				get_of_rel->get_cppdecl_a(), 
				get_of_rel->get_isa_const_relation_a(), 
				get_of_rel->get_multiplicity_a());
      else
	oper->update_cpp_get_of(decl, def, get_of_rel->get_role_b(), 
				get_of_rel->get_cppdecl_b(), 
				get_of_rel->get_isa_const_relation_b(), 
				get_of_rel->get_multiplicity_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_cpp_set_of(decl, def, set_of_rel->get_role_a(), 
				set_of_rel->get_cppdecl_a(), 
				set_of_rel->get_isa_const_relation_a(), 
				set_of_rel->get_multiplicity_a());
      else
	oper->update_cpp_set_of(decl, def, set_of_rel->get_role_b(), 
				set_of_rel->get_cppdecl_b(), 
				set_of_rel->get_isa_const_relation_b(), 
				set_of_rel->get_multiplicity_b());
    }

    edcppdecl->setText(decl);
  }
  else {
    QString s = oper->default_cpp_decl(edname->text().stripWhiteSpace());
    
    GenerationSettings::set_cpp_return_type(the_type(edreturn_type->currentText()
						     .stripWhiteSpace(),
						     list, nodes),
					    s);
    
    int index;
    QString params;
    int nparams = (int) table->nparams();
    QString sep;
    
    for (index = 0; index != nparams; index += 1, sep = ", ")
      params += sep + GenerationSettings::cpp(the_type(table->type(index),
						       list, nodes),
					      table->dir(index), index)
	+ "${v" + QString().setNum(index) + "}";
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edcppdecl->setText(s);
  }
  
  cpp_update_decl();
}

void OperationDialog::cpp_unmapped_decl() {
  edcppdecl->setText(QString::null);
  showcppdecl->setText(QString::null);

  cpp_unmapped_def();
}

void OperationDialog::cpp_decl_from_def() {
  QString dcl = oper->default_cpp_decl(edname->text().stripWhiteSpace());
  QString def = edcppdef->text();
  int index1;
  int index2;
  int index3;
  int index4;
  
  // copy args
  if (((index1 = def.find("${(}") + 4) == (-1 + 4)) ||
      ((index2 = def.find("${)}", index1)) == -1) ||
      ((index3 = dcl.find("${(}") + 4) == (-1 + 4)) ||
      ((index4 = dcl.find("${)}", index3)) == -1))
      // wrong specification(s)
    return;
  
  if (index4 != index3)
    dcl.remove(index3, index4 - index3);
  if (index2 != index1)
    dcl.insert(index3, def.mid(index1, index2 - index1));

  // copy return type modifications
  if (((index1 = def.find("${type}")) != -1) &&
      ((index2 = def.find("${class}", index1 + 7)) != -1) &&
      ((index3 = dcl.find("${type}")) != -1) &&
      ((index4 = dcl.find("${name}", index3 + 7)) != -1)) {
    // valid specification
    while ((index1 != 0) && (def.at(index1 - 1) != QChar('}')))
      index1 -= 1;
    while ((index3 != 0) && (dcl.at(index3 - 1) != QChar('}')))
      index3 -= 1;
    dcl.remove(index3, index4 - index3);
    dcl.insert(index3, def.mid(index1, index2 - index1));
  }

  // manage const
  if ((def.find("${const}") == -1) && // const removed
      ((index1 = dcl.find("${const}")) != -1))
    // remove const
    dcl.remove(index1, 8);

  // manage throw
  if ((def.find("${throw}") == -1) && // throw removed
      ((index1 = dcl.find("${throw}")) != -1))
    // remove throw
    dcl.remove(index1, 8);

  //add default param value
  index1 = 0;
  while (((index2 = dcl.find("${p", index1)) != -1) &&
	 ((index3 = dcl.find('}', index2 + 3)) != -1)) {
    dcl.insert(index3 + 1, "${v" + dcl.mid(index2 + 3, index3 - index2 - 2));
    index1 = index3 + index3 - index2 + 2;
  }
  
  // update decl
  edcppdecl->setText(dcl);
  cpp_update_decl();
}

void OperationDialog::cpp_edit_param_decl() {
  QString form = edcppdecl->text();
  int index;

  if (((index = form.find("${(}")) != 0) &&
      (form.find("${)}", index + 4) != 0)) {
    CppParamsDialog d(this, table, edcppdecl, TRUE);
    
    if (d.exec() == QDialog::Accepted)
      cpp_update_decl();
  }
  else
    msg_warning("Bouml", TR("wrong specification"));
}

void OperationDialog::manage_cpp_type(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += get_cpp_name(the_type(table->type(rank), list, nodes));
  else {
    s += "${t";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_var(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += table->name(rank);
  else {
    s += "${p";
    s += QString::number(rank);
    s += '}';
  }
}

QString OperationDialog::compute_name(LineEdit * spec) {
  if (spec != 0) {
    QString s = spec->text().stripWhiteSpace();
    int index;
    
    if ((index = s.find("${name}")) != -1)
      s.replace(index, 7, ((BrowserOperation *) oper->browser_node)->get_of_name());
    else if ((index = s.find("${Name}")) != -1)
      s.replace(index, 7,
		capitalize(((BrowserOperation *) oper->browser_node)->get_of_name()));
    return s;
  }
  else
    return edname->text();
}

void OperationDialog::manage_init(unsigned rank, QString & s)
{
  if (rank < table->nparams()) {
    QString v = table->value(rank).stripWhiteSpace();
    
    if (! v.isEmpty())
      s += " = " + v;
  }
  else {
    s += "${v";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_cpp_exceptions(QString & s)
{
  int index;
  int ne = etable->nexceptions();
  const char * sep = " throw (";
	
  for (index = 0; index != ne; index += 1) {
    s += sep;
    s += GenerationSettings::cpp_type(type(etable->type(index), list, nodes));
    sep = ", ";
  }
  
  if (index != 0)
    s += ')';
  else if (GenerationSettings::cpp_oper_force_throw())
    s += " throw ()";
}

void OperationDialog::cpp_update_decl() {
  cpp_visibility.update_default(uml_visibility);
  
  // do NOT write
  //	const char * p = edcppdecl->text();
  // because the QString is immediatly destroyed !
  QString def = edcppdecl->text();
  const char * p = def;
  const char * pp = 0;
  QString indent = "";
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  if (*p != '#')
    s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      if (*p != '#')
	s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp,
		     GenerationSettings::cpp_javadoc_style());
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${friend}", 9)) {
      p += 9;
      if (friend_cb->isChecked())
	s += "friend ";
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (classoperation_cb->isChecked())
	s += "static ";
    }
    else if (!strncmp(p, "${inline}", 9)) {
      p += 9;
      if (inline_cb->isChecked())
	s += "inline ";
    }
    else if (!strncmp(p, "${virtual}", 10)) {
      p += 10;
      if (virtual_cb->isChecked())
	s += "virtual ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_cpp_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				 list, nodes));
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += compute_name(edcppnamespec);
    }
    else if (!strncmp(p, "${class}", 8)) {
      // to be placed in the description
      p += 8;
      s += cl->get_browser_node()->get_name();
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (!strncmp(p, "${const}", 8)) {
      p += 8;
      if (const_cb->isChecked())
	s += " const";
    }
    else if (!strncmp(p, "${volatile}", 11)) {
      p += 11;
      if (volatile_cb->isChecked())
	s += " volatile";
    }
    else if (!strncmp(p, "${throw}", 8)) {
      p += 8;
      manage_cpp_exceptions(s);
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (abstract_cb->isChecked())
	s += " = 0";
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      manage_cpp_type(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      manage_var(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      manage_init(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (oper->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::cpp_relationattribute_stereotype(oper->stereotype);
    }
    else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::cpp_type(((BrowserOperation *) oper->browser_node)
					->get_of_association().get_type());
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p && (*p != '#'))
	s += indent;
    }
    else if (*p == '@')
      manage_alias(oper->browser_node, p, s, kvtable);
    else
      s += *p++;
  }

  showcppdecl->setText(s);
}

QString OperationDialog::cpp_decl(const BrowserOperation * op, bool withname,
				  ShowContextMode mode)
{
  OperationData * d = (OperationData *) op->get_data();
  QCString decl = d->cpp_decl;
  
  remove_comments(decl);
  remove_preprocessor(decl);
  
  const char * p = decl;
  bool afterparams = FALSE;
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${friend}", 9))
      p += 9;
    else if (!strncmp(p, "${static}", 9))
      p += 9;
    else if (!strncmp(p, "${inline}", 9))
      p += 9;
    else if (!strncmp(p, "${virtual}", 10))
      p += 10;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_cpp_name(d->return_type, mode);
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += op->compute_name(d->cpp_name_spec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
      afterparams = TRUE;
    }
    else if (!strncmp(p, "${const}", 8))
      p += 8;
    else if (!strncmp(p, "${volatile}", 11))
      p += 11;
    else if (!strncmp(p, "${throw}", 8))
      p += 8;
    else if (!strncmp(p, "${abstract}", 11)) 
      break;
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (rank < d->nparams) 
	s += GenerationSettings::cpp_type(d->params[rank].get_type().get_type(mode));
      else {
	s += "${t";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (withname) {
	if (rank < d->nparams) 
	  s += d->params[rank].get_name();
	else {
	  s += "${p";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      if (rank < d->nparams) {
	QCString v = d->params[rank].get_default_value();
	
	if (!v.isEmpty())
	  s += " = " + v;
      }
      else {
	s += "${v";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (d->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::cpp_relationattribute_stereotype(d->stereotype);
    }
    else if (d->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::cpp_type(op->get_of_association().get_type());
    }
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if ((*p == '{') ||
	     (*p == ';') ||
	     ((*p == ':') && afterparams))
      break;
    else if (*p == '@')
      manage_alias(op, p, s, 0);
    else
      s += *p++;
  }

  return s;
}

void OperationDialog::cpp_default_def() {
  if (oper->is_get_or_set) {
    QCString decl;
    QCString def;
    
    if (get_of_attr != 0)
      oper->update_cpp_get_of(decl, def, get_of_attr->get_browser_node()->get_name(),
			      get_of_attr->get_cppdecl(),
			      get_of_attr->get_isa_const_attribute(),
			      get_of_attr->get_multiplicity());
    else if (set_of_attr != 0)
      oper->update_cpp_set_of(decl, def, set_of_attr->get_browser_node()->get_name(),
			      set_of_attr->get_cppdecl(),
			      set_of_attr->get_isa_const_attribute(),
			      set_of_attr->get_multiplicity());
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_cpp_get_of(decl, def, get_of_rel->get_role_a(), 
				get_of_rel->get_cppdecl_a(), 
				get_of_rel->get_isa_const_relation_a(), 
				get_of_rel->get_multiplicity_a());
      else
	oper->update_cpp_get_of(decl, def, get_of_rel->get_role_b(), 
				get_of_rel->get_cppdecl_b(), 
				get_of_rel->get_isa_const_relation_b(), 
				get_of_rel->get_multiplicity_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_cpp_set_of(decl, def, set_of_rel->get_role_a(), 
				set_of_rel->get_cppdecl_a(), 
				set_of_rel->get_isa_const_relation_a(), 
				set_of_rel->get_multiplicity_a());
      else
	oper->update_cpp_set_of(decl, def, set_of_rel->get_role_b(), 
				set_of_rel->get_cppdecl_b(), 
				set_of_rel->get_isa_const_relation_b(), 
				set_of_rel->get_multiplicity_b());
    }

    edcppdef->setText(def);
  }
  else {
    QString s = oper->default_cpp_def(edname->text().stripWhiteSpace());
    
    GenerationSettings::set_cpp_return_type(the_type(edreturn_type->currentText()
						     .stripWhiteSpace(),
						     list, nodes),
					    s);
    
    QString params;
    int index;
    int nparams = (int) table->nparams();
    QString sep;
    
    for (index = 0; index != nparams; index += 1, sep = ", ")
      params += sep + GenerationSettings::cpp(the_type(table->type(index),
						       list, nodes),
					      table->dir(index), index);
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edcppdef->setText(s);
  }
  cpp_update_def();
}

void OperationDialog::cpp_unmapped_def() {
  edcppdef->setText(QString::null);
  showcppdef->setText(QString::null);
}

void OperationDialog::cpp_def_from_decl() {
  
  QString dcl = edcppdecl->text();
  QString def;
  
  // manage abstract
  if ((dcl.find("${abstract}") == -1) || // abstract removed
      !abstract_cb->isChecked()) {
    // probably not abstract
    int index1;
    int index2;
    int index3;
    int index4;
    
    def = oper->default_cpp_def(edname->text().stripWhiteSpace());
    
    // copy args
    if (((index1 = dcl.find("${(}") + 4) == (-1 + 4)) ||
	((index2 = dcl.find("${)}", index1)) == -1) ||
	((index3 = def.find("${(}") + 4) == (-1 + 4)) ||
	((index4 = def.find("${)}", index3)) == -1))
      // wrong specification(s)
      return;
    
    if (index4 != index3)
      def.remove(index3, index4 - index3);
    if (index2 != index1)
      def.insert(index3, dcl.mid(index1, index2 - index1));
    
    // copy return type modifications
    if (((index1 = dcl.find("${type}")) != -1) &&
	((index2 = dcl.find("${name}", index1 + 7)) != -1) &&
	((index3 = def.find("${type}")) != -1) &&
	((index4 = def.find("${class}", index3 + 7)) != -1)) {
      // valid specifications
      while ((index1 != 0) && (dcl.at(index1 - 1) != QChar('}')))
	index1 -= 1;
      while ((index3 != 0) && (def.at(index3 - 1) != QChar('}')))
	index3 -= 1;
      def.remove(index3, index4 - index3);
      def.insert(index3, dcl.mid(index1, index2 - index1));
    }
    
    // manage const
    if ((dcl.find("${const}") == -1) && // const removed
	((index1 = def.find("${const}")) != -1))
      // remove const
      def.remove(index1, 8);
    
    // manage throw
    if ((dcl.find("${throw}") == -1) && // throws removed
	((index1 = def.find("${throw}")) != -1))
      // remove throw
      def.remove(index1, 8);
    
    // remove inits
    index1 = def.find("${(}");
    index4 = def.find("${)}", index1 + 4);
    
    while (((index2 = def.find("${v", index1)) != -1) &&
	   (index2 < index4) &&
	   ((index3 = def.find('}', index2 + 3)) != -1)) {
      def.remove(index2, index3 - index2 + 1);
      index1 = index2;
    }
  }
  
  // update def
  edcppdef->setText(def);
  cpp_update_def();
}

void OperationDialog::cpp_edit_param_def() {
  QString form = edcppdef->text();
  int index;

  if (((index = form.find("${(}")) != 0) &&
      (form.find("${)}", index + 4) != 0)) {
    CppParamsDialog d(this, table, edcppdef, FALSE);
    
    d.raise();
    if (d.exec() == QDialog::Accepted)
      cpp_update_def();
  }
  else
    msg_warning("Bouml", TR("wrong specification"));
}

// return TRUE if stop on ${commnt} or ${description}
bool insert_template(const QString & tm, const char *& p, 
		     QString & s, const QString & indent)
{
  // search the beginning of the definition/declaration in p;
  for (;;) {
    if (*p == 0)
      return FALSE;
    
    if (*p == '\n') {
      s += *p++;
      if (*p != '#')
	s += indent;
    }
    if (*p <= ' ')
      s += *p++;
    else if (*p == '/') {
      if (p[1] == '/') {
	// comment
	s += *p++;
	do s += *p++;
	while (*p && (*p != '\n'));
      }
      else if (p[1] == '*') {
	/* comment */
	s += *p++;
	do s += *p++;
	while (*p && ((*p != '*') || (p[1] != '/')));
	s += "*/";
	p += 2;
      }
      else
	break;
    }
    else if (*p == '#') {
      do {
	s += *p++;
	if (*p == '\\') {
	  s += *p++;
	  s += *p++;
	}
	else if ((*p == '/') && (p[1] == '*')) {
	  /* comment */
	  s += *p++;
	  do s += *p++;
	  while (*p && ((*p != '*') || (p[1] != '/')));
	  s += "*/";
	  p += 2;
	}
      } while (*p && (*p != '\n'));
    }
    else if ((strncmp(p, "${comment}", 10) == 0) ||
	     (strncmp(p, "${description}", 14) == 0))
      return TRUE;
    else
      break;
  }
  
  s += tm;
  return FALSE;
}

void OperationDialog::cpp_update_def() {
  QString def = edcppdef->text();
  bool template_oper = edcppdecl->text().isEmpty()
    && oper->is_template_operation(def);
  QString s;
  
  if (!abstract_cb->isChecked()) {
    // do NOT write
    //	const char * p = edcppdef->text()
    // because the QString is immediatly destroyed !
    const char * p = def;
    const char * pp = 0;
    QString indent = "";
    unsigned rank;
    
    while ((*p == ' ') || (*p == '\t'))
      indent += *p++;
    
    bool re_template = !templates.isEmpty() &&
      insert_template((template_oper) ? templates_tmplop : templates, 
		      p, s, indent);
    
    if (*p != '#')
      s += indent;
  
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;

	if (re_template)
	  s += templates;

	if (*p == 0)
	  break;
	if (*p != '#')
	  s += indent;
      }
      
      if (!strncmp(p, "${comment}", 10)) {
	if (!manage_comment(comment->text(), p, pp,
			    GenerationSettings::cpp_javadoc_style())
	    && re_template)
	  s += templates;
      }
      else if (!strncmp(p, "${description}", 14)) {
	if (!manage_description(comment->text(), p, pp) && re_template)
	  s += templates;
      }
      else if (!strncmp(p, "${inline}", 9)) {
	p += 9;
	if (inline_cb->isChecked())
	  s += "inline ";
      }
      else if (!strncmp(p, "${type}", 7)) {
	p += 7;
	s += get_cpp_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				   list, nodes));
      }
      else if (!strncmp(p, "${class}", 8)) {
	p += 8;
	if (friend_cb->isChecked() && !strncmp(p, "::", 2))
	  p += 2;
	else
	  s += ((template_oper) ? cl_names_tmplop : cl_names);
      }
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	s += compute_name(edcppnamespec);
      }
      else if (!strncmp(p, "${(}", 4)) {
	p += 4;
	s += '(';
      }
      else if (!strncmp(p, "${)}", 4)) {
	p += 4;
	s += ')';
      }
      else if (!strncmp(p, "${const}", 8)) {
	p += 8;
	if (const_cb->isChecked())
	  s += " const";
      }
      else if (!strncmp(p, "${volatile}", 11)) {
	p += 11;
	if (volatile_cb->isChecked())
	  s += " volatile";
      }
      else if (!strncmp(p, "${throw}", 8)) {
	p += 8;
      manage_cpp_exceptions(s);
      }
      else if (!strncmp(p, "${staticnl}", 11)) {
	p += 11;
	if (classoperation_cb->isChecked()) {
	  s += '\n';
	  s += indent;
	}
	else
	  s += ' ';
      }
      else if (sscanf(p, "${t%u}", &rank) == 1) {
	manage_cpp_type(rank, s);
	p = strchr(p, '}') + 1;
      }
      else if (sscanf(p, "${p%u}", &rank) == 1) {
	manage_var(rank, s);
	p = strchr(p, '}') + 1;
      }
      else if (oper->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
	p += 13;
	// get/set with multiplicity > 1
	s += GenerationSettings::cpp_relationattribute_stereotype(oper->stereotype);
      }
      else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
	p += 14;
	// get/set with multiplicity > 1
	s += GenerationSettings::cpp_type(((BrowserOperation *) oper->browser_node)
					  ->get_of_association().get_type());
      }
      else if (*p == '\n') {
	s += *p++;
	if (*p && (*p != '#'))
	  s += indent;
      }
      else if (*p == '@')
	manage_alias(oper->browser_node, p, s, kvtable);
      else
	s += *p++;
    }
    
    editcppbody->setEnabled(def.find("${body}") != -1);
  }
  else
    editcppbody->setEnabled(FALSE);
  
  showcppdef->setText(s);
  
  forcegenbody_toggled(forcegenbody_cb->isChecked());	// update indent*body_cb
}

static QString Marker = " ---- header including this line will be automatically removed ----\n";

static QString add_profile(QString b, const char * comment = "//")
{
  return b.left(b.find("${body}")) + comment + Marker;
}

static QString remove_profile(QString b)
{
  int index = b.find(Marker);
  
  return (index == -1)
    ? b
    : b.mid(index + Marker.length());
}

void OperationDialog::cpp_edit_body() {
  QString b;

  if (add_operation_profile())
    b = add_profile(showcppdef->text()) + cppbody;
  else
    b = cppbody;

  edit(b, edname->text().stripWhiteSpace() + "_body",
       oper, CppEdit, this, 
       (preserve_bodies() && !forcegenbody_cb->isChecked())
       ? (post_edit) 0
       : (post_edit) post_cpp_edit_body,
       edits);
}

void OperationDialog::post_cpp_edit_body(OperationDialog * d, QString s)
{
  d->cppbody = (add_operation_profile()) ? remove_profile(s) : s;
}

// Java

void OperationDialog::java_finalsynchronized_toggled(bool) {
  java_update_def();
}

void OperationDialog::manage_java_type(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += get_java_name(the_type(table->type(rank), list, nodes));
  else {
    s += "${t";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_java_exceptions(QString & s)
{
  int index;
  int ne = etable->nexceptions();
  const char * sep = " throws ";
	
  for (index = 0; index != ne; index += 1) {
    s += sep;
    s += GenerationSettings::java_type(type(etable->type(index), list, nodes));
    sep = ", ";
  }
  
  if (index != 0)
    s += ' ';
}

void OperationDialog::java_default_def() {
  if (oper->is_get_or_set) {
    QCString def;
    
    if (get_of_attr != 0)
      oper->update_java_get_of(def, get_of_attr->get_browser_node()->get_name(),
			       get_of_attr->get_javadecl(),
			       get_of_attr->get_multiplicity());
    else if (set_of_attr != 0)
      oper->update_java_set_of(def, set_of_attr->get_browser_node()->get_name(),
			       set_of_attr->get_javadecl(),
			       set_of_attr->get_multiplicity());
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_java_get_of(def, get_of_rel->get_role_a(), 
				 get_of_rel->get_javadecl_a(),
				 get_of_rel->get_multiplicity_a());
      else
	oper->update_java_get_of(def, get_of_rel->get_role_b(), 
				 get_of_rel->get_javadecl_b(),
				 get_of_rel->get_multiplicity_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_java_set_of(def, set_of_rel->get_role_a(), 
				 set_of_rel->get_javadecl_a(),
				 set_of_rel->get_multiplicity_a());
      else
	oper->update_java_set_of(def, set_of_rel->get_role_b(), 
				 set_of_rel->get_javadecl_b(),
				 set_of_rel->get_multiplicity_b());
    }

    edjavadef->setText(def);
  }
  else {
    QString s = oper->default_java_def(edname->text().stripWhiteSpace());
    QString params;
    int index;
    int nparams = (int) table->nparams();
    const char * sep;
    
    for (index = 0, sep = ""; index != nparams; index += 1, sep = ", ") {
      QString p;
      
      p.sprintf("%s${t%d} ${p%d}", sep, index, index);
      params += p;
    }
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edjavadef->setText(s);
  }
  
  java_update_def();
}

void OperationDialog::java_unmapped_def() {
  edjavadef->setText(QString::null);
  showjavadef->setText(QString::null);
}

static const char * bypass_body(const char * p)
{
  // p point to '{'
  
  p += 1;
  
  const char * pb = strstr(p, "${body}");
  
  if (pb != 0) {
    pb += 7;
    while (*pb)
      if (*pb++ == '}')
	return pb;
  }
  
  return p;
}

void OperationDialog::java_update_def() {
  // do NOT write
  //	const char * p = edjavadef->text();
  // because the QString is immediatly destroyed !
  QString def = edjavadef->text();
  QString ste = GenerationSettings::java_class_stereotype(cl->get_stereotype());
  bool interf = (ste == "interface") || (ste == "@interface");
  bool nobody = (abstract_cb->isChecked() || interf);
  const char * p = def;
  const char * pp = 0;
  const char * afterparam = 0;
  QString indent = "";
  QString s;
  unsigned rank;
  
  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp,
		     GenerationSettings::java_javadoc_style());
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (javafinal_cb->isChecked())
	s += "final ";
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      if (uml_visibility.value() != UmlPackageVisibility)
	s += uml_visibility.state() + ' ';
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (classoperation_cb->isChecked())
	s += "static ";
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (abstract_cb->isChecked() && !interf)
	s += "abstract ";
    }
    else if (!strncmp(p, "${synchronized}", 15)) {
      p += 15;
      if (synchronized_cb->isChecked())
	s += "synchronized ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_java_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				  list, nodes));
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += compute_name(edjavanamespec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      afterparam = p;
      s += ')';
    }
    else if (!strncmp(p, "${throws}", 9)) {
      manage_java_exceptions(s);
      p += 9;
    }
    else if (!strncmp(p, "${staticnl}", 11)) {
      p += 11;
      if (classoperation_cb->isChecked()) {
	s += '\n';
	s += indent;
      }
      else
	s += ' ';
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      manage_java_type(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      manage_var(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (oper->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::java_relationattribute_stereotype(oper->stereotype);
    }
    else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::java_type(((BrowserOperation *) oper->browser_node)
					 ->get_of_association().get_type());
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p)
	s += indent;
    }
    else if ((*p == '{') && nobody) {
      if (afterparam != 0) {
	if ((ste == "@interface") &&
	    (strstr(afterparam, "default") != 0))
	  afterparam = 0;
	else {
	  s += ';';
	  p = bypass_body(p);
	  continue;
	}
      }
      s += *p++;
    }
    else if (!strncmp(p, "${@}", 4)) {
      p += 4;
      if (pp != 0)
	s += "${@}";
      else if (! javaannotation.isEmpty()) {
	pp =p;
	p = javaannotation;
      }
    }
    else if (*p == '@')
      manage_alias(oper->browser_node, p, s, kvtable);
    else
      s += *p++;
  }
    
  editjavabody->setEnabled(!nobody && (def.find("${body}") != -1));
  editjavaannotation->setEnabled(def.find("${@}") != -1);  
  
  showjavadef->setText(s);
  
  forcegenbody_toggled(forcegenbody_cb->isChecked());	// update indent*body_cb
}

QString OperationDialog::java_decl(const BrowserOperation * op, bool withname,
				   ShowContextMode mode)
{
  OperationData * d = (OperationData *) op->get_data();
  QCString decl = d->java_def;
  
  remove_comments(decl);
  
  const char * p = decl;
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${final}", 8))
      p += 8;
    else if (!strncmp(p, "${visibility}", 13))
      p += 13;
    else if (!strncmp(p, "${static}", 9))
      p += 9;
    else if (!strncmp(p, "${abstract}", 11))
      p += 11;
    else if (!strncmp(p, "${synchronized}", 15))
      p += 15;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_java_name(d->return_type, mode);
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += op->compute_name(d->java_name_spec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (!strncmp(p, "${throws}", 9))
      p += 9;
    else if (!strncmp(p, "${staticnl}", 11)) {
      p += 11;
      s += ' ';
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (rank < d->nparams) 
	s += GenerationSettings::java_type(d->params[rank].get_type().get_type(mode));
      else {
	s += "${t";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (withname) {
	if (rank < d->nparams) 
	  s += d->params[rank].get_name();
	else {
	  s += "${p";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (!strncmp(p, "${@}", 4))
      p += 4;
    else if (d->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::java_relationattribute_stereotype(d->stereotype);
    }
    else if (d->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::java_type(op->get_of_association().get_type());
    }
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if ((*p == '{') || (*p == ';'))
      break;
    else if (*p == '@')
      manage_alias(op, p, s, 0);
    else
      s += *p++;
  }
  
  return s;
}

void OperationDialog::java_edit_body() {
  QString b;

  if (add_operation_profile())
    b = add_profile(showjavadef->text()) + javabody;
  else
    b = javabody;

  edit(b, edname->text().stripWhiteSpace() + "_body",
       oper, JavaEdit, this, 
       (preserve_bodies() && !forcegenbody_cb->isChecked())
       ? (post_edit) 0
       : (post_edit) post_java_edit_body,
       edits);
}

void OperationDialog::post_java_edit_body(OperationDialog * d, QString s)
{
  d->javabody = (add_operation_profile()) ? remove_profile(s) : s;
}

void OperationDialog::java_edit_annotation() {
  AnnotationDialog dialog(this, javaannotation, !hasOkButton());
  
  if (dialog.exec() == QDialog::Accepted)
    java_update_def();
}

// Php

void OperationDialog::php_final_toggled(bool) {
  php_update_def();
}

void OperationDialog::manage_php_type(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += get_php_name(the_type(table->type(rank), list, nodes));
  else {
    s += "${t";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::php_default_def() {
  if (oper->is_get_or_set) {
    QCString def;
    
    if (get_of_attr != 0)
      oper->update_php_get_of(def, get_of_attr->get_browser_node()->get_name(),
			       get_of_attr->get_phpdecl());
    else if (set_of_attr != 0)
      oper->update_php_set_of(def, set_of_attr->get_browser_node()->get_name(),
			       set_of_attr->get_phpdecl());
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_php_get_of(def, get_of_rel->get_role_a(), 
				 get_of_rel->get_phpdecl_a());
      else
	oper->update_php_get_of(def, get_of_rel->get_role_b(), 
				 get_of_rel->get_phpdecl_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_php_set_of(def, set_of_rel->get_role_a(), 
				 set_of_rel->get_phpdecl_a());
      else
	oper->update_php_set_of(def, set_of_rel->get_role_b(), 
				 set_of_rel->get_phpdecl_b());
    }

    edphpdef->setText(def);
  }
  else {
    QString ste = GenerationSettings::php_class_stereotype(cl->get_stereotype());
    QString s = oper->default_php_def(edname->text().stripWhiteSpace(),
				      abstract_cb->isChecked() || (ste == "interface"));
    QString params;
    int index;
    int nparams = (int) table->nparams();
    const char * sep;
    
    for (index = 0, sep = ""; index != nparams; index += 1, sep = ", ") {
      QString p;
      
      p.sprintf("%s${p%d}${v%d}", sep, index, index);
      params += p;
    }
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edphpdef->setText(s);
  }
  
  php_update_def();
}

void OperationDialog::php_unmapped_def() {
  edphpdef->setText(QString::null);
  showphpdef->setText(QString::null);
}

void OperationDialog::php_update_def() {
  // do NOT write
  //	const char * p = edphpdef->text();
  // because the QString is immediatly destroyed !
  QString def = edphpdef->text();
  QString ste = GenerationSettings::php_class_stereotype(cl->get_stereotype());
  bool interf = (ste == "interface");
  bool nobody = (abstract_cb->isChecked() || interf);
  const char * p = def;
  const char * pp = 0;
  const char * afterparam = 0;
  QString indent = "";
  QString s;
  unsigned rank;
  
  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp,
		     GenerationSettings::php_javadoc_style());
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (phpfinal_cb->isChecked())
	s += "final ";
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      if (uml_visibility.value() != UmlPackageVisibility)
	s += uml_visibility.state() + ' ';
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (classoperation_cb->isChecked())
	s += "static ";
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (abstract_cb->isChecked() && !interf)
	s += "abstract ";
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += compute_name(edphpnamespec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      afterparam = p;
      s += ')';
    }
    else if (!strncmp(p, "${staticnl}", 11)) {
      p += 11;
      if (classoperation_cb->isChecked()) {
	s += '\n';
	s += indent;
      }
      else
	s += ' ';
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      manage_php_type(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      s += "$";
      manage_var(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      manage_init(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p)
	s += indent;
    }
    else if ((*p == '{') && nobody) {
      s += ";";
      break;
    }
    else if (*p == '@')
      manage_alias(oper->browser_node, p, s, kvtable);
    else if (!strncmp(p, "${type}", 7)) {
      // for comment
      p += 7;
      s += get_php_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				 list, nodes));
    }
    else
      s += *p++;
  }
    
  editphpbody->setEnabled(!nobody && (def.find("${body}") != -1));
  
  showphpdef->setText(s);
  
  forcegenbody_toggled(forcegenbody_cb->isChecked());	// update indent*body_cb
}

QString OperationDialog::php_decl(const BrowserOperation * op, bool withname,
				  ShowContextMode mode)
{
  OperationData * d = (OperationData *) op->get_data();
  QCString decl = d->php_def;
  
  remove_comments(decl);
  
  int index = decl.find("function ");
  if (index != -1)
    decl.remove(index, 9);
  
  const char * p = decl;
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${final}", 8))
      p += 8;
    else if (!strncmp(p, "${visibility}", 13))
      p += 13;
    else if (!strncmp(p, "${static}", 9))
      p += 9;
    else if (!strncmp(p, "${abstract}", 11))
      p += 11;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += op->compute_name(d->php_name_spec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (!strncmp(p, "${staticnl}", 11)) {
      p += 11;
      s += ' ';
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (rank < d->nparams) 
	s += d->params[rank].get_type().get_type(mode);
      else {
	s += "${t";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (withname) {
	if (rank < d->nparams) 
	  s += QCString("$") + d->params[rank].get_name();
	else {
	  s += "${p";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      if (rank < d->nparams) {
	QCString v = d->params[rank].get_default_value();
	
	if (!v.isEmpty())
	  s += " = " + v;
      }
      else {
	s += "${v";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if ((*p == '{') || (*p == ';'))
      break;
    else if (*p == '@')
      manage_alias(op, p, s, 0);
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_php_name(d->return_type, mode);
    }
    else
      s += *p++;
  }
  
  return s;
}

void OperationDialog::php_edit_body() {
  QString b;

  if (add_operation_profile())
    b = add_profile(showphpdef->text()) + phpbody;
  else
    b = phpbody;

  edit(b, edname->text().stripWhiteSpace() + "_body",
       oper, PhpEdit, this, 
       (preserve_bodies() && !forcegenbody_cb->isChecked())
       ? (post_edit) 0
       : (post_edit) post_php_edit_body,
       edits);
}

void OperationDialog::post_php_edit_body(OperationDialog * d, QString s)
{
  d->phpbody = (add_operation_profile()) ? remove_profile(s) : s;
}

void OperationDialog::php_edit_param() {
  QString form = edphpdef->text();
  int index;

  if (((index = form.find("${(}")) != 0) &&
      (form.find("${)}", index + 4) != 0)) {
    PhpParamsDialog d(this, table, edphpdef);
    
    if (d.exec() == QDialog::Accepted)
      php_update_def();
  }
  else
    msg_warning("Bouml", TR("wrong specification"));
}

// Python

void OperationDialog::manage_python_type(unsigned rank, QString & s, bool in_param)
{
  if (rank < table->nparams()) {
    QString t = get_python_name(the_type(table->type(rank), list, nodes));
    
    if (!t.isEmpty()) {
      if (in_param)
	s += ": " + t;
      else
	s += t;
    }
  }
  else {
    s += "${t";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::python_default_def() {
  if (oper->is_get_or_set) {
    QCString def;
    
    if (get_of_attr != 0)
      oper->update_python_get_of(def, get_of_attr->get_browser_node()->get_name(),
				 get_of_attr->get_pythondecl(),
				 get_of_attr->get_isa_class_attribute());
    else if (set_of_attr != 0)
      oper->update_python_set_of(def, set_of_attr->get_browser_node()->get_name(),
				 set_of_attr->get_pythondecl(),
				 set_of_attr->get_isa_class_attribute());
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_python_get_of(def, get_of_rel->get_role_a(), 
				   get_of_rel->get_pythondecl_a(),
				   get_of_rel->get_isa_class_relation_a());
      else
	oper->update_python_get_of(def, get_of_rel->get_role_b(), 
				   get_of_rel->get_pythondecl_b(),
				   get_of_rel->get_isa_class_relation_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_python_set_of(def, set_of_rel->get_role_a(), 
				 set_of_rel->get_pythondecl_a(),
				 set_of_rel->get_isa_class_relation_a());
      else
	oper->update_python_set_of(def, set_of_rel->get_role_b(), 
				 set_of_rel->get_pythondecl_b(),
				 set_of_rel->get_isa_class_relation_b());
    }

    edpythondef->setText(def);
  }
  else {
    // rmq : abstractmethod have body !
    QString s = oper->default_python_def(edname->text().stripWhiteSpace());
    QString params;
    int index;
    int nparams = (int) table->nparams();
    const char * sep;
    
    if (GenerationSettings::python_default_operation3()) {
      for (index = 0, sep = ""; index != nparams; index += 1, sep = ", ") {
	QString p;
	
	p.sprintf("%s${p%d}${t%d}${v%d}", sep, index, index, index);
	params += p;
      }
    }
    else {
      for (index = 0, sep = ""; index != nparams; index += 1, sep = ", ") {
	QString p;
	
	p.sprintf("%s${p%d}${v%d}", sep, index, index);
	params += p;
      }
    }
    
    int index2;
    
    if (((index = s.find("${(}")+4) != (-1+4)) &&
	((index2 = s.find("${)}", index)) != -1)) {
      int index3 = s.find("${type}", index2+4);
      int index4 = s.find(":", index2+4);
      
      if (GenerationSettings::python_default_operation3()) {
	if ((index3 == -1) || (index3 > index4))
	  s.insert(index2+4, "${type}");
      }
      else if ((index3 != -1) && (index3 < index4))
	s.remove(index3, 7);
	       
      if (index2 != index)
	s.remove(index, index2-index);
      s.insert(index, params);
    }
    
    edpythondef->setText(s);
  }
  
  python_update_def();
}

void OperationDialog::python_unmapped_def() {
  edpythondef->setText(QString::null);
  showpythondef->setText(QString::null);
}

void OperationDialog::manage_decorators(QString & s, QString indent,
					BooL & indent_needed) {
  if (! pythondecorator.isEmpty()) {
    int index = 0;
    int index2;
    
    while ((index2 = pythondecorator.find("\n", index)) != -1){
      if (indent_needed)
	s += indent;
      else
	indent_needed = TRUE;
      s += pythondecorator.mid(index, index2 + 1 - index);
      index = index2 + 1;
    }
    
    if (index != (int) pythondecorator.length()) {
      if (indent_needed) {
	s += indent;
	indent_needed = FALSE;
      }
      s += pythondecorator.mid(index);
    }
  }
}

void OperationDialog::python_update_def() {
  // do NOT write
  //	const char * p = edpythondef->text();
  // because the QString is immediatly destroyed !
  QString def = edpythondef->text();
  const char * p = def;

  if (p == 0)
    return;
  
  bool nobody = abstract_cb->isChecked();
  const char * pp = 0;
  const char * afterparam = 0;
  BooL indent_needed = FALSE;
  bool in_params = FALSE;
  QString indent;
  QString saved_indent = indent;
  QString indent_step = 
    GenerationSettings::python_get_indent_step();
  QString s;
  unsigned rank;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      if (*p == 0)
	break;
      pp = 0;
      indent = saved_indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_python_comment(comment->text(), p, pp);
    else if (!strncmp(p, "${description}", 14))
      manage_python_description(comment->text(), p, pp);
    else if (!strncmp(p, "${docstring}", 12))
      manage_python_docstring(comment->text(), p, pp, indent_needed, indent, saved_indent);
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (classoperation_cb->isChecked()) {
	if (indent_needed)
	  s += "\n";
	else
	  indent_needed = TRUE;
	s += "@staticmethod\n";
      }
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (abstract_cb->isChecked()) {
	if (indent_needed)
	  s += "\n";
	else
	  indent_needed = TRUE;
	s += "@abstractmethod\n";
      }
    }
    else if (!strncmp(p, "${name}", 7)) {
      if (indent_needed) {
	indent_needed = FALSE;
	s += indent;
      }
      p += 7;
      s += compute_name(edpythonnamespec);
    }
    else if (!strncmp(p, "${class}", 8)) {
      if (indent_needed) {
	indent_needed = FALSE;
	s += indent;
      }
      p += 8;
      s += cl->get_browser_node()->get_name();
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
      in_params = TRUE;
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      afterparam = p;
      s += ')';
      in_params = FALSE;
    }
    else if (!strncmp(p, "${@}", 4)) {
      manage_decorators(s, indent, indent_needed);
      p += 4;
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      manage_python_type(rank, s, in_params);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      manage_var(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      manage_init(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (*p == '@')
      manage_alias(oper->browser_node, p, s, kvtable);
    else if (!strncmp(p, "${body}", 7)) {
      if (edname->text().stripWhiteSpace() == "__init__")
	s += ClassDialog::python_instance_att_rel(cl->get_browser_node());
      s += "${body}";
      p += 7;
      indent_needed = FALSE;
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      
      QString t = get_python_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
					   list, nodes));
      
      if (!t.isEmpty())
	s += " -> " + t;
    }
    else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      if (indent_needed) {
	indent_needed = FALSE;
	s += indent;
      }
      // get/set with multiplicity > 1
      s += ((BrowserOperation *) oper->browser_node)
	->get_of_association().get_type();
    }
    else {
      if (indent_needed) {
	indent_needed = FALSE;
	s += indent;
      }
      switch (*p) {
      case ':':
	if (pp == 0) {
	  indent += indent_step;
	  saved_indent = indent;
	  indent_step = "";
	}
	break;
      case '\n':
	indent_needed = TRUE;
	break;
      }
      s += *p++;
    }
  }
    
  editpythonbody->setEnabled(!nobody && (def.find("${body}") != -1));
  editpythondecorator->setEnabled(def.find("${@}") != -1);  
  
  showpythondef->setText(s);
  
  forcegenbody_toggled(forcegenbody_cb->isChecked());	// update indent*body_cb
}

QString OperationDialog::python_decl(const BrowserOperation * op, bool withname,
				     ShowContextMode mode)
{
  OperationData * d = (OperationData *) op->get_data();
  QCString decl = d->python_def;
  bool in_params = FALSE;
  
  remove_python_comments(decl);
  
  int index = decl.find("def ");
  QString s;

  if (index == -1)
    return s;

  decl.remove(0, index + 4);
  
  const char * p = decl;
  unsigned rank;

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${docstring}", 12))
      p += 12;
    else if (!strncmp(p, "${static}", 9))
      p += 9;
    else if (!strncmp(p, "${abstract}", 11))
      p += 11;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += op->compute_name(d->python_name_spec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
      in_params = TRUE;
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
      
      if (!strncmp(p, "${type}", 7)) {
	QString t = get_python_name(d->return_type, mode);
	
	if (!t.isEmpty())
	  s += " -> " + t;
      }
      break;
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (rank < d->nparams) {
	QString t = d->params[rank].get_type().get_type(mode);
	
	if (!t.isEmpty()) {
	  if (in_params)
	    s += ": " + t;
	  else
	    s += t;
	}
      }
      else {
	s += "${t";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (withname) {
	if (rank < d->nparams) 
	  s += d->params[rank].get_name();
	else {
	  s += "${p";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      if (rank < d->nparams) {
	QCString v = d->params[rank].get_default_value();
	
	if (!v.isEmpty())
	  s += " = " + v;
      }
      else {
	s += "${v";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (d->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += op->get_of_association().get_type();
    }
    else if (*p == '@')
      manage_alias(op, p, s, 0);
    else if (!strncmp(p, "${type}", 7)) {
      // probably in comment
      p += 7;
    }
    else
      s += *p++;
  }
  
  return s;
}

void OperationDialog::python_edit_body() {
  QString b;

  if (add_operation_profile())
    b = add_profile(showpythondef->text(), "#") + pythonbody;
  else
    b = pythonbody;

  edit(b, edname->text().stripWhiteSpace() + "_body",
       oper, PythonEdit, this, 
       (preserve_bodies() && !forcegenbody_cb->isChecked())
       ? (post_edit) 0
       : (post_edit) post_python_edit_body,
       edits);
}

void OperationDialog::post_python_edit_body(OperationDialog * d, QString s)
{
  d->pythonbody = (add_operation_profile()) ? remove_profile(s) : s;
}

void OperationDialog::python_edit_param() {
  QString form = edpythondef->text();
  int index;

  if (((index = form.find("${(}")) != 0) &&
      (form.find("${)}", index + 4) != 0)) {
    PythonParamsDialog d(this, table, edpythondef);
    
    if (d.exec() == QDialog::Accepted)
      python_update_def();
  }
  else
    msg_warning("Bouml", TR("wrong specification"));
}

void OperationDialog::python_edit_decorator() {
  DecoratorDialog dialog(this, pythondecorator, !hasOkButton());
  
  if (dialog.exec() == QDialog::Accepted)
    python_update_def();
}

// Idl

void OperationDialog::oneway_toggled(bool) {
  idl_update_decl();
}

void OperationDialog::manage_idl_type(unsigned rank, QString & s)
{
  if (rank < table->nparams()) 
    s += get_idl_name(the_type(table->type(rank), list, nodes));
  else {
    s += "${t";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_dir(unsigned rank, QString & s)
{
  if (rank < table->nparams()) {
    switch (table->dir(rank)) {
    case UmlIn:
      s += "in";
      break;
    case UmlOut:
      s += "out";
      break;
    default:
      // can't be return
      s += "inout";
    }
  }
  else {
    s += "${d";
    s += QString::number(rank);
    s += '}';
  }
}

void OperationDialog::manage_idl_exceptions(QString & s)
{
  int index;
  int ne = etable->nexceptions();
  const char * sep = " raises (";
	
  for (index = 0; index != ne; index += 1) {
    s += sep;
    s += GenerationSettings::idl_type(type(etable->type(index), list, nodes));
    sep = ", ";
  }
  
  if (index != 0)
    s += ')';
}

void OperationDialog::idl_default_def() {
  if (oper->is_get_or_set) {
    QCString decl;
    
    if (get_of_attr != 0)
      oper->update_idl_get_of(decl, get_of_attr->get_idldecl(),
			      get_of_attr->get_multiplicity());
    else if (set_of_attr != 0)
      oper->update_idl_set_of(decl, set_of_attr->get_idldecl(),
			      set_of_attr->get_multiplicity());
    else if (get_of_rel != 0) {
      if (is_rel_a)
	oper->update_idl_get_of(decl, get_of_rel->get_idldecl_a(),
				get_of_rel->get_multiplicity_a());
      else
	oper->update_idl_get_of(decl, get_of_rel->get_idldecl_b(),
				get_of_rel->get_multiplicity_b());
    }
    else {
      // set_of_rel != 0
      if (is_rel_a)
	oper->update_idl_set_of(decl, set_of_rel->get_idldecl_a(),
				set_of_rel->get_multiplicity_a());
      else
	oper->update_idl_set_of(decl, set_of_rel->get_idldecl_b(),
				set_of_rel->get_multiplicity_b());
    }

    edidldecl->setText(decl);
  }
  else {
    QString s = oper->default_idl_decl(edname->text().stripWhiteSpace());
    QString params;
    int index;
    int nparams = (int) table->nparams();
    const char * sep;
    
    for (index = 0, sep = ""; index != nparams; index += 1, sep = ", ") {
      QString p;
      
      p.sprintf("%s${d%d} ${t%d} ${p%d}", sep, index, index, index);
      params += p;
    }
    
    if ((index = s.find("${)}")) != -1)
      s.insert(index, params);
    
    edidldecl->setText(s);
  }
  
  idl_update_decl();
}

void OperationDialog::idl_unmapped_def() {
  edidldecl->setText(QString::null);
  showidldecl->setText(QString::null);
}

void OperationDialog::idl_update_decl() {  
  // do NOT write
  //	const char * p = edidldecl->text();
  // because the QString is immediatly destroyed !
  QString def = edidldecl->text();
  const char * p = def;
  const char * pp = 0;
  QString indent = "";
  QString s;
  unsigned rank;
  
  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  s = indent;
  
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      s += indent;
    }
      
    if (!strncmp(p, "${comment}", 10))
      manage_comment(comment->text(), p, pp, FALSE);
    else if (!strncmp(p, "${description}", 14))
      manage_description(comment->text(), p, pp);
    else if (!strncmp(p, "${oneway}", 9)) {
      p += 9;
      if (oneway_cb->isChecked())
	s += "oneway ";
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_idl_name(the_type(edreturn_type->currentText().stripWhiteSpace(),
				 list, nodes));
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += compute_name(edidlnamespec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (sscanf(p, "${d%u}", &rank) == 1) {
      manage_dir(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      manage_idl_type(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      manage_var(rank, s);
      p = strchr(p, '}') + 1;
    }
    else if (oper->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::idl_relationattribute_stereotype(oper->stereotype);
    }
    else if (oper->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::idl_type(((BrowserOperation *) oper->browser_node)
					->get_of_association().get_type());
    }
    else if (!strncmp(p, "${raisesnl}", 11)) {
      if (etable->nexceptions() != 0) {
	s += '\n';
	s += indent;
	s += "  ";
      }
      p += 11;
    }
    else if (!strncmp(p, "${raises}", 9)) {
      manage_idl_exceptions(s);
      p += 9;
    }
    else if (*p == '\n') {
      s += *p++;
      if (*p)
	s += indent;
    }
    else if (*p == '@')
      manage_alias(oper->browser_node, p, s, kvtable);
    else
      s += *p++;
  }
  
  showidldecl->setText(s);
}

QString OperationDialog::idl_decl(const BrowserOperation * op, bool withdir,
				  bool withname, ShowContextMode mode)
{
  OperationData * d = (OperationData *) op->get_data();
  QCString decl = d->idl_decl;
  
  remove_comments(decl);
  
  const char * p = decl;
  QString s;
  unsigned rank;

  while ((*p == ' ') || (*p == '\t'))
    p += 1;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${oneway}", 9))
      p += 9;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      s += get_idl_name(d->return_type, mode);
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      s += op->compute_name(d->idl_name_spec);
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      s += '(';
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      s += ')';
    }
    else if (sscanf(p, "${d%u}", &rank) == 1) {
      if (withdir) {
	if (rank < d->nparams) 
	  switch (d->params[rank].get_dir()) {
	  case UmlIn:
	    s += "in";
	    break;
	  case UmlOut:
	    s += "out";
	    break;
	  default:
	    // can't be return
	    s += "inout";
	}
	else {
	  s += "${d";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      if (withname) {
	if (rank < d->nparams) 
	  s += GenerationSettings::idl_type(d->params[rank].get_type().get_type(mode));
	else {
	  s += "${t";
	  s += QString::number(rank);
	  s += '}';
	}
      }
      p = strchr(p, '}') + 1;
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      if (rank < d->nparams) 
	s += d->params[rank].get_name();
      else {
	s += "${p";
	s += QString::number(rank);
	s += '}';
      }
      p = strchr(p, '}') + 1;
    }
    else if (d->is_get_or_set && !strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set with multiplicity > 1
      s += GenerationSettings::idl_relationattribute_stereotype(d->stereotype);
    }
    else if (d->is_get_or_set && !strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set with multiplicity > 1
      s += GenerationSettings::idl_type(op->get_of_association().get_type());
    }
    else if (!strncmp(p, "${raisesnl}", 11)) {
      p += 11;
      s += ' ';
    }
    else if (!strncmp(p, "${raises}", 9))
      p += 9;
    else if (*p == '\n') {
      s += ' ';
      do
	p+= 1;
      while ((*p == ' ') || (*p == '\t'));
    }
    else if (*p == ';')
      break;
    else if (*p == '@')
      manage_alias(op, p, s, 0);
    else
      s += *p++;
  }
  
  return s;
}

// automatic add / remove param when only one language is set

static int bypass_string(const char * s, int index)
{
  // index is just after the "
  for (;;) {
    switch (s[index]) {
    case '"':
      return index + 1;
    case '\\':
      if (s[index + 1] == 0)
	return index + 1;
      index += 2;
      break;
    case 0:
      return index;
    default:
      index += 1;
    }
  }
}

static int bypass_char(const char * s, int index)
{
  // index is just after the '
  for (;;) {
    switch (s[index]) {
    case '\'':
      return index + 1;
    case '\\':
      if (s[index + 1] == 0)
	return index + 1;
      index += 2;
      break;
    case 0:
      return index;
    default:
      index += 1;
    }
  }
}

static int bypass_cpp_comment(const char * s, int index)
{
  // index is just after the //
  const char * p = strchr(s, '\n');
  
  return (p == 0)
    ? index + strlen(s + index)
    : p - s;
}

static int bypass_c_comment(const char * s, int index)
{
  // index is just after the /*
  for (;;) {
    switch (s[index]) {
    case '*':
      if (s[index + 1] == '/')
	return index + 2;
      break;
    case 0:
      return index;
    }
    index += 1;
  }
}

static int supOf(const char * s, int index)
{
  // return the index after of the parameter form
  // s at least contains ${)}
  int ouvr = 0;
  
  for (;;) {
    switch (s[index]) {
    case '$':
      if (strncmp(s + index, "${)}", 4) == 0)
	return index;
      index += 1;
      break;
    case '(':
    case '[':
    case '{':
      ouvr += 1;
      index += 1;
      break;
    case ')':
    case ']':
    case '}':
      ouvr -= 1;
      index += 1;
      break;
    case '"':
      index = bypass_string(s, index + 1);
      break;
    case '\'':
      index = bypass_char(s, index + 1);
      break;
    case '/':
      switch (s[index+1]) {
      case '/':
	index = bypass_cpp_comment(s, index + 2);
	break;
	case '*':
	index = bypass_c_comment(s, index + 2);
	break;
      default:
	index += 1;
      }
      break;
    case 0:
      // in case ${)} is in a comment etc ...
      return index;
    case ',':
      if (ouvr == 0)
	return index;
      // no break
    default:
      index += 1;
    }
  }
}

static int param_begin(QString s, int rank)
{
  // return position of ',' or '}' (inside ${(}), 
  // or '$' (inside ${)}) or -1 on error
  const char * p = s;
  const char * b = strstr(p, "${(}");
  
  if ((b == 0) || (strstr(b+4, "${)}") == 0))
    return -1;
  
  int index  = (b - p) + 3;	// '}'
  
  while (rank != 0) {
    int end = supOf(p, index + 1);
    
    switch (p[end]) {
    case ',':
      index = end;
      break;
    case '$': // ${)}
      return (rank == 1) ? end : -1;
    default:
      return -1;
    }
    
    rank -= 1;
  }
  
  return index;
}

static void renumber(QString & form, int rank,
		     int delta, bool equal = FALSE)
{
  int index = form.find("${(}");
  
  if (index == -1)
    return;
  
  index += 4;
  
  int index_sup = form.find("${)}", index);
  
  while (index < index_sup) {
    index = form.find("${", index);
    if (index == -1)
      break;
    
    int index2 = form.find('}', index + 3);
    QString n = form.mid(index + 3, index2 - index - 3);
    bool ok = FALSE;
    int r = n.toInt(&ok);
    
    if (!ok || ((equal) ? (r != rank) : (r < rank)))
      index = index2 + 1;
    else {
      char nn[16];
      
      sprintf(nn, "%d", r + delta);
      form.replace(index + 3, n.length(), nn);
      index = form.find('}', index + 3) + 1;
    }
  }
}

void OperationDialog::force_param(int rank, bool recompute) {
  char t[16];
  char p[16];
  char v[16];
  QString s;
  
  sprintf(t, "${t%d}", rank);
  sprintf(p, "${p%d}", rank);
  sprintf(v, "${v%d}", rank);
  
  switch (unique) {
  case CppView:
    {
      QString theo =
	GenerationSettings::cpp(the_type(table->type(rank), list, nodes),
				table->dir(rank), rank);
      
      s = edcppdecl->text();
      if ((s.find(t) == -1) && (s.find(p) == -1)) {
	add_param(s, rank, theo+v);
	edcppdecl->setText(s);
      }	
      else if (recompute) {
	replace_param(s, rank, theo);
	edcppdecl->setText(s);
      }
      
      s = edcppdef->text();
      if ((s.find(t) == -1) && (s.find(p) == -1)) {
	add_param(s, rank, theo);
	edcppdef->setText(s);
      }	
      else if (recompute) {
	replace_param(s, rank, theo);
	edcppdef->setText(s);
      }
    }
    break;
  case JavaView:
    s = edjavadef->text();
    if ((s.find(t) == -1) && (s.find(p) == -1)) {
      add_param(s, rank, QString(t) + " " + p);
      edjavadef->setText(s);
    }
    break;
  case PhpView:
    s = edphpdef->text();
    if (s.find(p) == -1) {
      add_param(s, rank, QString(p) + v);
      edphpdef->setText(s);
    }
    break;
  case PythonView:
    s = edpythondef->text();
    if (s.find(p) == -1) {
      add_param(s, rank, QString(p) + v);
      edpythondef->setText(s);
    }
    break;
  case IdlView:
    s = edidldecl->text();
    if ((s.find(t) == -1) && (s.find(p) == -1)) {
      char d[16];
  
      sprintf(d, "${d%d}", rank);
      add_param(s, rank, QString(d) + " " + t + " " + p);
      edidldecl->setText(s);
    }
    break;
  default:
    break;
  }
}

void OperationDialog::add_param(QString & form, int rank, QString s) {
  int index = param_begin(form, rank);
  
  if (index == -1)
    return;
  
  if (rank == 0) {
    // first param, index point to '}' ending ${(}
    if (table->nparams() != 1)
      s += ", ";
    index += 1;
  }
  else
    // index point to ',' or '$' starting ${)}
    s = ", " + s;
  
  form.insert(index, s);
}

void OperationDialog::replace_param(QString & form, int rank, QString s) {
  int index_start = param_begin(form, rank);
  
  if (index_start == -1)
    return;
  
  const char * p = form;
  
  switch (p[index_start]) {
  case '$':
    // '$' starting ${)}, param not yet present, not first one,
    // not possible except if user remove it in language form
    form.insert(index_start, ", " + s);
    return;
  case ',':
    index_start += (p[index_start + 1] == ' ') ? 2 : 1;
    break;
  default:
    // first param, index point to '}' ending ${(}
    index_start += 1;
  }
  
  int index_sup = supOf(p, index_start);
  
  switch (p[index_sup]) {
  case ',':
  case '$': // ${)}
    break;
  default:
    // error
    return;
  }
  
  form.replace(index_start, index_sup - index_start, s);
}

void OperationDialog::insert_param(int rank) {
  // just renumber
  switch (unique) {
  case CppView:
    insert_param(rank, edcppdecl);
    insert_param(rank, edcppdef);
    break;
  case JavaView:
    insert_param(rank, edjavadef);
    break;
  case PhpView:
    insert_param(rank, edphpdef);
    break;
  case PythonView:
    insert_param(rank, edpythondef);
    break;
  case IdlView:
    insert_param(rank, edidldecl);
    break;
  default:
    break;
  }
}

void OperationDialog::insert_param(int rank, MultiLineEdit * ed) {
  // just renumber
  QString form = ed->text();
  
  renumber(form, rank, 1);
  ed->setText(form);
}

void OperationDialog::delete_param(int rank) {
  // remove and renumber
  switch (unique) {
  case CppView:
    delete_param(rank, edcppdecl);
    delete_param(rank, edcppdef);
    break;
  case JavaView:
    delete_param(rank, edjavadef);
    break;
  case PhpView:
    delete_param(rank, edphpdef);
    break;
  case PythonView:
    delete_param(rank, edpythondef);
    break;
  case IdlView:
    delete_param(rank, edidldecl);
    break;
  default:
    break;
  }
}

QString OperationDialog::delete_param(int rank, MultiLineEdit * ed) {
  // remove
  QString form = ed->text();
  int index = param_begin(form, rank);
  
  if (index == -1)
    return "";
  
  const char * p = form;
  int index_sup;
  
  switch (p[index]) {
  case '$':
    // '$' starting ${)}, param not yet present, not first one,
    // not possible except if user remove it in language form
    return "";
  case ',':
    index_sup = supOf(p, index + 1);
    break;
  default: // first param, index point to '}' ending ${(}
    index += 1;
    index_sup = supOf(p, index);
    break;
  }
  
  QString result;
  
  switch (p[index_sup]) {
  case ',':
    if (p[index] == ',') {
      if (p[index + 1] == ' ')
	result = form.mid(index + 2, index_sup - index - 2);
      else
	result = form.mid(index + 1, index_sup - index - 1);
    }
    else {
      // first param
      result = form.mid(index, index_sup - index);
      index_sup += (p[index_sup + 1] == ' ') ? 2 : 1;
    }
    break;
  case '$': // ${)}
    if (p[index] == ',') {
      if (p[index + 1] == ' ')
	result = form.mid(index + 2, index_sup - index - 2);
      else
	result = form.mid(index + 1, index_sup - index - 1);
    }
    else {
      // alone param
      result = form.mid(index, index_sup - index);
    }
    break;
  default:
    // error
    return "";
  }
  
  form.remove(index, index_sup - index);
  
  // renumber
  renumber(form, rank, -1);
  
  ed->setText(form);
  return result;
}

void OperationDialog::move_param(int old_rank, int new_rank) {
  switch (unique) {
  case CppView:
    move_param(old_rank, new_rank, edcppdecl);
    move_param(old_rank, new_rank, edcppdef);
    break;
  case JavaView:
    move_param(old_rank, new_rank, edjavadef);
    break;
  case PhpView:
    move_param(old_rank, new_rank, edphpdef);
    break;
  case PythonView:
    move_param(old_rank, new_rank, edpythondef);
    break;
  case IdlView:
    move_param(old_rank, new_rank, edidldecl);
    break;
  default:
    break;
  }
}

void OperationDialog::move_param(int old_rank, int new_rank,
				 MultiLineEdit * ed) {
  QString s = delete_param(old_rank, ed);
  
  if (s.isEmpty())
    return;
  
  s = "${(}" + s + "${)}";
  renumber(s, old_rank, new_rank - old_rank, TRUE);

  QString form = ed->text();
  
  renumber(form, new_rank, 1);
  add_param(form, new_rank, s.mid(4, s.length() - 8));
  
  ed->setText(form);
}

//
// ParamTable
//

// copy/cut/paste
QString ParamsTable::dir_copy;
QString ParamsTable::name_copy;
QString ParamsTable::type_copy;
QString ParamsTable::default_value_copy;

static QStringList DirList;

ParamsTable::ParamsTable(OperationData * o, QWidget * parent,
			 const QStringList & list,
			 OperationDialog * d, bool visit)
    : MyTable(o->get_n_params() + 1, (visit) ? 4 : 5, parent),
      dialog(d), types(list) {
  int index;
  int sup = o->get_n_params();
    
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, TR("Direction"));
  horizontalHeader()->setLabel(1, TR("Name"));
  horizontalHeader()->setLabel(2, TR("Type"));
  horizontalHeader()->setLabel(3, TR("Default value"));
  
  if (visit) {
    for (index = 0; index < sup; index += 1){
      setItem(index, 0, new TableItem(this, QTableItem::Never, stringify(o->get_param_dir(index))));
      setItem(index, 1, new TableItem(this, QTableItem::Never, o->get_param_name(index)));
      setItem(index, 2, new TableItem(this, QTableItem::Never, o->get_param_type(index).get_full_type()));
      setItem(index, 3, new TableItem(this, QTableItem::Never, o->get_param_default_value(index)));
    }
  }
  else {
    horizontalHeader()->setLabel(4, TR("do"));
    
    alltypes = GenerationSettings::basic_types();
    ((ClassData *) ((BrowserNode *) o->get_browser_node()->parent())->get_data())
      ->addFormals(alltypes);
    for (QStringList::ConstIterator it = list.begin(); it != list.end(); ++it)
      alltypes.append(*it);
    
    if (DirList.count() == 0) {
      // Made DirList without be linked with the items order
      DirList.append(QString::null);
      DirList.append(QString::null);
      DirList.append(QString::null);
      DirList[UmlInOut] = stringify(UmlInOut);
      DirList[UmlIn] = stringify(UmlIn);
      DirList[UmlOut] = stringify(UmlOut);
      // can't be return
    }
    
    for (index = 0; index < sup; index += 1){
      setItem(index, 0, new ComboItem(this, stringify(o->get_param_dir(index)), DirList, FALSE));
      setText(index, 1, o->get_param_name(index));
      setItem(index, 2, new ComboItem(this, o->get_param_type(index).get_full_type(), alltypes));
      setText(index, 3, o->get_param_default_value(index));
      setText(index, 4, QString::null);
    }
    
    setItem(index, 0, new ComboItem(this, stringify(UmlIn), DirList, FALSE));
    setText(index, 1, QString::null);
    setItem(index, 2, new ComboItem(this, QString::null, alltypes));
    setText(index, 3, QString::null);
    setText(index, 4, QString::null);
  }
  
  adjustColumn(0);
  //setColumnStretchable (1, TRUE);
  horizontalHeader()->setResizeEnabled(TRUE, 1);
  setColumnStretchable (2, TRUE);
  //setColumnStretchable (3, TRUE);
  horizontalHeader()->setResizeEnabled(TRUE, 3);
  
  if (! visit) {
    adjustColumn(4);
    setColumnStretchable (4, FALSE);
    
    connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	    this, SLOT(button_pressed(int, int, int, const QPoint &)));
    connect(this, SIGNAL(valueChanged(int, int)),
	    this, SLOT(value_changed(int, int)));
  }
}

void ParamsTable::activateNextCell() {
  int row = currentRow();
  int col = currentColumn();
    
  if (col == 3) {
    // goto next line
    if (++row == numRows()) {
      // adds a new line
      setNumRows(row + 1);
      setItem(row, 0, new ComboItem(this, stringify(UmlIn), DirList, FALSE));
      setText(row, 1, QString::null);
      setItem(row, 2, new ComboItem(this, QString::null, alltypes));
      setText(row, 3, QString::null);
      setText(row, 4, QString::null);
    }
    setCurrentCell(row, 0);
  }
  else
    setCurrentCell(row, col + 1);
}

void ParamsTable::value_changed(int row, int col) {
  if ((row == (numRows() - 1)) && (col != 0) && !text(row, col).isEmpty())
    insert_row_after(row);
  
  if ((col <= 2) && (!text(row, 1).isEmpty() || !text(row, 2).isEmpty()))
    dialog->force_param(row, col != 1);
}

void ParamsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 4) {
    int n = nparams();
    char s[16];
    QPopupMenu m;
    
    sprintf(s, "%d", row + 1);
    m.insertItem(TR("param %1", s), -1);
    m.insertSeparator();
    if (row < n) {
      m.insertItem(TR("Insert param before"), 0);
      m.insertItem(TR("Insert param after"), 1);
      m.insertSeparator();
      m.insertItem(TR("Delete param"), 2);
      m.insertSeparator();
      m.insertItem(TR("Copy param"), 3);
      m.insertItem(TR("Cut param"), 4);
    }
    if (!name_copy.isEmpty() || !type_copy.isEmpty())
      m.insertItem(TR("Paste param"), 5);

    QPopupMenu mv;
    int rank;
    
    if (row < n) {
      for (rank = 0; rank != n; rank += 1)
	if (rank != row)
	  mv.insertItem(QString::number(rank + 1), 10 + rank);
    
      if (mv.count() != 0) {
	m.insertSeparator();
	m.insertItem(TR("Move param"), &mv);
      }
    }
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      dialog->insert_param(row);
      dialog->force_param(row, TRUE);
      break;
    case 1:
      insert_row_after(row);
      dialog->insert_param(row+1);
      dialog->force_param(row+1, TRUE);
      break;
    case 2:
      delete_row(row);
      dialog->delete_param(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      dialog->delete_param(row);
      break;
    case 5:
      paste_row(row);
      dialog->force_param(row, TRUE);
      break;
    default:
      if (rank >= 10) {
	move_row(row, rank - 10);
	dialog->move_param(row, rank - 10);
      }
      break;
    }
  }
}

void ParamsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    QTableItem * it;
    
    it = item(index - 1, 0);
    takeItem(it);
    setItem(index, 0, it);
    setText(index, 1, text(index - 1, 1));
    it = item(index - 1, 2);
    takeItem(it);
    setItem(index, 2, it);
    setText(index, 3, text(index - 1, 3));
  }
  
  setItem(row, 0, new ComboItem(this, stringify(UmlIn), DirList, FALSE));
  setText(row, 1, QString::null);
  setItem(row, 2, new ComboItem(this, QString::null, alltypes));
  setText(row, 3, QString::null);
  setText(row, 4, QString::null);
 }

void ParamsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    QTableItem * it;
    
    it = item(index - 1, 0);
    takeItem(it);
    setItem(index, 0, it);
    setText(index, 1, text(index - 1, 1));
    it = item(index - 1, 2);
    takeItem(it);
    setItem(index, 2, it);
    setText(index, 3, text(index - 1, 3));
  }
  
  setItem(row + 1, 0, new ComboItem(this, stringify(UmlIn), DirList, FALSE));
  setText(row + 1, 1, QString::null);
  setItem(row + 1, 2, new ComboItem(this, QString::null, alltypes));
  setText(row + 1, 3, QString::null);
  setText(row + 1, 4, QString::null);
 }

void ParamsTable::delete_row(int row) {
  int n = numRows();
  int index;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    setItem(row, 0, new ComboItem(this, stringify(UmlIn), DirList, FALSE));
    setText(row, 1, QString::null);
    setItem(row, 2, new ComboItem(this, QString::null, alltypes));
    setText(row, 3, QString::null);
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      QTableItem * it;
      
      it = item(index + 1, 0);
      takeItem(it);
      setItem(index, 0, it);
      setText(index, 1, text(index + 1, 1));
      it = item(index + 1, 2);
      takeItem(it);
      setItem(index, 2, it);
      setText(index, 3, text(index + 1, 3));
    }
    setNumRows(n - 1);
  }
}

void ParamsTable::copy_row(int row) {
  dir_copy = text(row, 0);
  name_copy = text(row, 1);
  type_copy = text(row, 2);
  default_value_copy = text(row, 3);
}

void ParamsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void ParamsTable::paste_row(int row) {
  setText(row, 0, dir_copy);
  setText(row, 1, name_copy);
  setText(row, 2, type_copy);
  setText(row, 3, default_value_copy);
  
  if ((row == (numRows() - 1)) &&
      (!dir_copy.isEmpty() ||
       !name_copy.isEmpty() ||
       !type_copy.isEmpty()))
    insert_row_after(row);
}

void ParamsTable::move_row(int from, int to) {
  QString save_dir_copy = dir_copy;
  QString save_name_copy = name_copy;
  QString save_type_copy = type_copy;
  QString save_default_value_copy = default_value_copy;
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  dir_copy = save_dir_copy;
  name_copy = save_name_copy;
  type_copy = save_type_copy;
  default_value_copy = save_default_value_copy;
}

void ParamsTable::update(OperationData * oper,
			 BrowserNodeList & nodes) {
  forceUpdateCells();
  
  int n = nparams();
  int index;
  
  oper->set_n_params(n);
  
  for (index = 0; index != n; index += 1) {    
    oper->set_param_dir(index, (UmlParamDirection) DirList.findIndex(text(index, 0)));
			
    oper->set_param_name(index, text(index, 1).stripWhiteSpace());
    
    AType t;
    QString s = text(index, 2).stripWhiteSpace();
    
    if (! s.isEmpty()) {
      int rank = types.findIndex(s);
      
      if (rank != -1)
	t.type = (BrowserClass *) nodes.at(rank);
      else
	t.explicit_type = s;
    }
     
    oper->set_param_type(index, t);
    
    oper->set_param_default_value(index, text(index, 3).stripWhiteSpace());
  }
}

unsigned ParamsTable::nparams() const {
  int n = numRows();
  
  while (n && text(n - 1, 1).isEmpty() && text(n - 1, 2).isEmpty())
    n -= 1;
  
  return n;
}

QString ParamsTable::name(unsigned rank) const {
  return text(rank, 1);
}

QString ParamsTable::type(unsigned rank) const {
  return text(rank, 2);
}

UmlParamDirection ParamsTable::dir(unsigned rank) const {
  return (UmlParamDirection) DirList.findIndex(text(rank, 0));
}

QString ParamsTable::value(unsigned rank) const {
  return text(rank, 3);
}

//
// ExceptionTable
//

// copy/cut/paste
QString ExceptionsTable::type_copy;

ExceptionsTable::ExceptionsTable(OperationData * o, QWidget * parent,
				 const QStringList & list, bool visit)
    : MyTable(o->get_n_exceptions() + 1, (visit) ? 1 : 2, parent), types(list) {
  int index;
  int sup = o->get_n_exceptions();
  
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, TR("Type"));
  
  if (visit) {
    for (index = 0; index < sup; index += 1)
      setItem(index, 0, new TableItem(this, QTableItem::Never, o->get_exception(index).get_full_type()));
  }
  else {
    horizontalHeader()->setLabel(1, TR("do"));
    
    for (index = 0; index < sup; index += 1){
      setItem(index, 0, new ComboItem(this, o->get_exception(index).get_full_type(), types));
      setText(index, 1, QString::null);
    }
    
    setItem(index, 0, new ComboItem(this, QString::null, types));
    setText(index, 1, QString::null);
  }
  
  setColumnStretchable (0, TRUE);
  
  if (! visit) {
    adjustColumn(1);
    setColumnStretchable (1, FALSE);
    
    connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	    this, SLOT(button_pressed(int, int, int, const QPoint &)));
    connect(this, SIGNAL(valueChanged(int, int)),
	    this, SLOT(value_changed(int, int)));
  }
}

void ExceptionsTable::activateNextCell() {
  int row = currentRow();
    
  // goto next line
  if (++row == numRows()) {
    // adds a new line
    setNumRows(row + 1);
    setItem(row, 0, new ComboItem(this, QString::null, types));
    setText(row, 1, QString::null);
  }
  setCurrentCell(row, 0);
}

void ExceptionsTable::value_changed(int row, int col) {
  if ((row == (numRows() - 1)) && !text(row, col).isEmpty())
    insert_row_after(row);
}

void ExceptionsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 1) {
    int n = nexceptions();
    char s[16];
    QPopupMenu m;
    
    sprintf(s, "%d", row + 1);
    m.insertItem(TR("exception %1", s), -1);
    m.insertSeparator();
    if (row < n) {
      m.insertItem(TR("Insert exception before"), 0);
      m.insertItem(TR("Insert exception after"), 1);
      m.insertSeparator();
      m.insertItem(TR("Delete exception"), 2);
      m.insertSeparator();
      m.insertItem(TR("Copy exception"), 3);
      m.insertItem(TR("Cut exception"), 4);
    }
    if (!type_copy.isEmpty())
      m.insertItem(TR("Paste exception"), 5);

    QPopupMenu mv;
    int rank;
    
    if (row < n) {
      for (rank = 0; rank != n; rank += 1)
	if (rank != row)
	  mv.insertItem(QString::number(rank + 1), 10 + rank);
    
      if (mv.count() != 0) {
	m.insertSeparator();
        m.insertItem(TR("Move exception"), &mv);
      }
    }
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      break;
    case 1:
      insert_row_after(row);
      break;
    case 2:
      delete_row(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      break;
    case 5:
      paste_row(row);
      break;
    default:
      if (rank >= 10)
	move_row(row, rank - 10);
      break;
    }
  }
}

void ExceptionsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    QTableItem * it = item(index - 1, 0);
    
    takeItem(it);
    setItem(index, 0, it);
  }
  
  setItem(row, 0, new ComboItem(this, QString::null, types));
  setText(row, 1, QString::null);
 }

void ExceptionsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    QTableItem * it = item(index - 1, 0);
    
    takeItem(it);
    setItem(index, 0, it);
  }
  
  setItem(row + 1, 0, new ComboItem(this, QString::null, types));
  setText(row + 1, 1, QString::null);
 }

void ExceptionsTable::delete_row(int row) {
  int n = numRows();
  int index;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    setItem(0, 0, new ComboItem(this, QString::null, types));
    setText(0, 1, QString::null);
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      QTableItem * it = item(index + 1, 0);
      takeItem(it);
      setItem(index, 0, it);
    }
    setNumRows(n - 1);
  }
}

void ExceptionsTable::copy_row(int row) {
  type_copy = text(row, 0);
}

void ExceptionsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void ExceptionsTable::paste_row(int row) {
  setText(row, 0, type_copy);
  
  if ((row == (numRows() - 1)) && !type_copy.isEmpty())
    insert_row_after(row);
}

void ExceptionsTable::move_row(int from, int to) {
  QString save_type_copy = type_copy;
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  type_copy = save_type_copy;
}

void ExceptionsTable::update(OperationData * oper,
			     BrowserNodeList & nodes) {
  forceUpdateCells();
  
  int n = nexceptions();
  int index;
  
  oper->set_n_exceptions(n);
  
  for (index = 0; index != n; index += 1) {   
    AType t;
    QString s = text(index, 0).stripWhiteSpace();
    
    if (!s.isEmpty()) {
      int rank = types.findIndex(s);
    
      if (rank != -1) 
	t.type = (BrowserClass *) nodes.at(rank);
      else
	t.explicit_type = s;
    }
    oper->set_exception(index, t);
  }
}

unsigned ExceptionsTable::nexceptions() const {
  int n = numRows();
  
  while (n && text(n - 1, 0).isEmpty())
    n -= 1;
  
  return n;
}

QString ExceptionsTable::type(unsigned rank) const {
  return text(rank, 0);
}

//
// CppParamTable
//

// copy/cut/paste
QString CppParamsTable::copied[8];	// copy/cut/paste

static QStringList SpecifierList;
static QStringList TypeRankList;
static QStringList PtrList;
static QStringList ParamRankList;
static QStringList ValueRankList;

CppParamsTable::CppParamsTable(ParamsTable * p, MultiLineEdit * f,
			       QWidget * parent, bool decl)
    : MyTable(0, (decl) ? 8 : 7, parent), params(p), edform(f), dcl(decl) {
    
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, TR("Name"));
  horizontalHeader()->setLabel(1, TR("Specifier"));
  horizontalHeader()->setLabel(2, "${t<i>}");
  horizontalHeader()->setLabel(3, TR("Pointer"));
  horizontalHeader()->setLabel(4, "${p<i>}");
  if (decl) {
    horizontalHeader()->setLabel(5, "${v<i>}");
    horizontalHeader()->setLabel(6, TR("Modifier"));
    horizontalHeader()->setLabel(7, TR("do"));
  }
  else {
    horizontalHeader()->setLabel(5, TR("Modifier"));
    horizontalHeader()->setLabel(6, TR("do"));
  }
  setColumnStretchable (0, TRUE);
  setColumnStretchable (1, TRUE);
  setColumnStretchable (2, TRUE);
  adjustColumn(3);
  setColumnStretchable (4, TRUE);
  setColumnStretchable (5, TRUE);
  if (decl) {
    setColumnStretchable (6, TRUE);
    adjustColumn(7);
    setColumnStretchable (7, FALSE);
  }
  else {
    adjustColumn(6);
    setColumnStretchable (6, FALSE);
  }
  
  QString form = edform->text();
  //the presence of ${(} and ${)} was checked
  int form_index = form.find("${(}") + 4;
  int tbl_index = 0;
    
  while (extract(tbl_index, form_index, form)) {
    setText(tbl_index, (decl) ? 7 : 6, QString::null);
    tbl_index += 1;
  }
  
  if (tbl_index == 0)
    insert_row_before(0);
    
  connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	  this, SLOT(button_pressed(int, int, int, const QPoint &)));
  
  if (SpecifierList.isEmpty()) {
    SpecifierList.append("");
    SpecifierList.append("const");
    SpecifierList.append("volatile");
  }
  
  if (PtrList.isEmpty()) {
    PtrList.append("");
    PtrList.append("*");
    PtrList.append("&");
    PtrList.append("*&");
  }
  
  TypeRankList.clear();
  ParamRankList.clear();
  ValueRankList.clear();
  
  for (int rank = 0; rank != params->numRows(); rank += 1) {
    if (!params->name(rank).isEmpty() || !params->type(rank).isEmpty()) {
      QString s;
      
      s.sprintf("${t%u}", rank);
      TypeRankList.append(s);
      s.sprintf("${p%u}", rank);
      ParamRankList.append(s);
      s.sprintf("${v%u}", rank);
      ValueRankList.append(s);
    }
  }
}

void CppParamsTable::init_row(int row) {
  setItem(row, 0, new QTableItem(this, QTableItem::Never, QString::null));
  setItem(row, 1, new ComboItem(this, QString::null, SpecifierList));
  setItem(row, 2, new ComboItem(this, QString::null, TypeRankList));
  setItem(row, 3, new ComboItem(this, QString::null, PtrList));
  setItem(row, 4, new ComboItem(this, QString::null, ParamRankList));
  if (dcl) {
    setItem(row, 5, new ComboItem(this, QString::null, ValueRankList));
    setText(row, 6, QString::null);
    setText(row, 7, QString::null);
  }
  else {
    setText(row, 5, QString::null);
    setText(row, 6, QString::null);
  }
}

bool CppParamsTable::extract(int tblindex, int & strindex, QString s) {
  // s at least contains ${)}
  while (s.at(strindex).isSpace())
    strindex += 1;
  
  int sup = supOf(s, strindex);
  
  if (s.mid(strindex, sup - strindex).stripWhiteSpace().isEmpty())
    return FALSE;
  
  QString specifier;
  QString t_i;
  QString ptr;
  QString p_i;
  QString modifier;
  int index = s.find("${t", strindex);
  
  if ((index == -1) || (index >= sup)) {
    // no $type
    index = s.find("${p", strindex);
    
    if ((index == -1) || (index >= sup))
      // no $type, no $name
      specifier = s.mid(strindex, sup - strindex).stripWhiteSpace();
    else {
      // no $type
      t_i = s.mid(strindex, index - strindex).stripWhiteSpace();
      
      // extract ptr
      int ip = t_i.find('*');
      int ir = t_i.find('&');

      if (ip != -1) {
	if ((ir != -1) && (ir < ip)) {
	  ptr = t_i.mid(ir).stripWhiteSpace();
	  t_i = t_i.left(ir).stripWhiteSpace();
	}
	else  {
	  ptr = t_i.mid(ip).stripWhiteSpace();
	  t_i = t_i.left(ip).stripWhiteSpace();
	}
      }
      else if (ir != -1) {
	ptr = t_i.mid(ir).stripWhiteSpace();
	t_i = t_i.left(ir).stripWhiteSpace();
      }
      
      if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
	return FALSE;
      else {
	strindex += 1;
	p_i = s.mid(index, strindex - index);
	modifier = s.mid(strindex, sup - strindex).stripWhiteSpace();
      }
    }
  }
  else {
    specifier = s.mid(strindex, index - strindex).stripWhiteSpace();

    if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
      return FALSE;
  
    strindex += 1;
    t_i = s.mid(index, strindex - index);
    
    index = s.find("${p", strindex);
    
    if ((index == -1) || (index >= sup))
      // unnamed
      modifier = s.mid(strindex, sup - strindex).stripWhiteSpace();
    else {
      ptr = s.mid(strindex, index - strindex).stripWhiteSpace();
      if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
	return FALSE;
      else {
	strindex += 1;
	p_i = s.mid(index, strindex - index);
	modifier = s.mid(strindex, sup - strindex).stripWhiteSpace();
      }
    }
  }
  
  setNumRows(tblindex + 1);

  setItem(tblindex, 1, new ComboItem(this, specifier, SpecifierList));
  setItem(tblindex, 2, new ComboItem(this, t_i, TypeRankList));
  setItem(tblindex, 3, new ComboItem(this, ptr, PtrList));
  setItem(tblindex, 4, new ComboItem(this, p_i, ParamRankList));
  if (dcl) {
    QString v_i;
    
    if ((modifier.length() >= 5) && (modifier.left(3) == "${v")) {
      if ((strindex = modifier.find('}', 3)) == -1)
	return FALSE;
      else {
	strindex += 1;
	v_i = modifier.left(strindex);
	modifier = modifier.mid(strindex);
      }
    }

    setItem(tblindex, 5, new ComboItem(this, v_i, ValueRankList));  
    setText(tblindex, 6, modifier);
  }
  else
    setText(tblindex, 5, modifier);
  
  strindex = (s.at(sup) == QChar(',')) ? sup + 1 : sup;
    
  return TRUE;
}

void CppParamsTable::setItem(int row, int col, QTableItem * item) {
  QTable::setItem(row, col, item);
  
  if ((col == 2) || (col == 4))
    update_name(row);
}

void CppParamsTable::setCurrentCell(int row, int col) {
  QTable::setCurrentCell(row, col);
  update_names();
}

void CppParamsTable::update_names() {
  int n = numRows();
  int row;

  for (row = 0; row != n; row += 1)
    update_name(row);
}

void CppParamsTable::update_name(int row) {
  bool t_set;
  bool p_set;
  unsigned t_i;
  unsigned p_i;
  
  if (!text(row, 2).isEmpty() &&
      (sscanf((const char *) text(row, 2), "${t%u}", &t_i) == 1))
    t_set = TRUE;
  else
    t_set = FALSE;
  
  if (!text(row, 4).isEmpty() &&
      (sscanf((const char *) text(row, 4), "${p%u}", &p_i) == 1))
    p_set = TRUE;
  else
    p_set = FALSE;
  
  if (t_set) {
    if (p_set)
      QTable::setItem(row, 0,
		      new QTableItem(this, QTableItem::Never,
				     ((t_i == p_i) && (t_i < params->nparams()))
				     ? params->name(t_i) : QString::null));
    else
      QTable::setItem(row, 0,
		      new QTableItem(this, QTableItem::Never,
				     (t_i < params->nparams())
				     ? params->name(t_i) : QString::null));
  }
  else
    QTable::setItem(row, 0,
		    new QTableItem(this, QTableItem::Never,
				   (p_set && (p_i < params->nparams()))
				   ? params->name(p_i) : QString::null));
}

void CppParamsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == ((dcl) ? 7 : 6)) {
    char s[16];
    QPopupMenu m;
    
    sprintf(s, "%d", row + 1);
    m.insertItem(TR("param %1", s), -1);
    m.insertSeparator();
    m.insertItem(TR("Insert param before"), 0);
    m.insertItem(TR("Insert param after"), 1);
    m.insertSeparator();
    m.insertItem(TR("Delete param"), 2);
    m.insertSeparator();
    m.insertItem(TR("Copy param"), 3);
    m.insertItem(TR("Cut param"), 4);
    m.insertItem(TR("Paste param"), 5);
    m.insertSeparator();
    
    QPopupMenu mv;
    int rank;
    
    for (rank = 0; rank != numRows(); rank += 1)
      if (rank != row)
	mv.insertItem(QString::number(rank + 1), 10 + rank);
    
    m.insertItem(TR("Move param"), &mv);
    m.insertSeparator();
    
    QPopupMenu rk;
    int t_i;
    int p_i;
    int v_i;
    
    if (text(row, 2).isEmpty() ||
	(sscanf((const char *) text(row, 2), "${t%d}", &t_i) != 1))
      t_i = -1;
    if (text(row, 4).isEmpty() ||
	(sscanf((const char *) text(row, 4), "${p%d}", &p_i) != 1))
      p_i = -1;
    if (!dcl ||
	text(row, 5).isEmpty() ||
	(sscanf((const char *) text(row, 5), "${v%d}", &v_i) != 1))
      v_i = -1;
    
    for (rank = 0; rank != params->numRows(); rank += 1)
      if ((!params->name(rank).isEmpty() || !params->type(rank).isEmpty()) &&
	  ((rank != t_i) || (rank != p_i)))
	rk.insertItem(QString::number(rank), 100 + rank);
    
    m.insertItem(TR("Set rank <i>"), &rk);
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      break;
    case 1:
      insert_row_after(row);
      break;
    case 2:
      delete_row(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      break;
    case 5:
      paste_row(row);
      break;
    default:
      if (rank >= 100) {
	char s[32];
	
	if (t_i != -1) {
	  sprintf(s, "${t%d}", rank - 100);
	  setItem(row, 2, new ComboItem(this, s, TypeRankList));
	}
	
	if (p_i != -1) {
	  sprintf(s, "${p%d}", rank - 100);
	  setItem(row, 4, new ComboItem(this, s, ParamRankList));
	}
	
	if (v_i != -1) {
	  sprintf(s, "${v%d}", rank - 100);
	  setItem(row, 5, new ComboItem(this, s, ParamRankList));
	}
      }
      else if (rank >= 10)
	move_row(row, rank - 10);
      break;
    }
  }
}

void CppParamsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  int col;
  int mcol = ((dcl) ? 6 : 5);
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    for (col = 0; col != mcol; col += 1) {
      QTableItem * it = item(index - 1, col);
      
      takeItem(it);
      setItem(index, col, it);
    }
    setText(index, mcol, text(index - 1, mcol));
    setText(index, mcol+1, text(index - 1, mcol+1));
  }

  init_row(row);
 }

void CppParamsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  int col;
  int mcol = ((dcl) ? 6 : 5);
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    for (col = 0; col != mcol; col += 1) {
      QTableItem * it = item(index - 1, col);
      
      takeItem(it);
      setItem(index, col, it);
    }
    setText(index, mcol, text(index - 1, col));
    setText(index, mcol+1, text(index - 1, col));
  }

  init_row(row + 1);
 }

void CppParamsTable::delete_row(int row) {
  int n = numRows();
  int index;
  int col;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    init_row(row);
  }
  else {
    int mcol = ((dcl) ? 6 : 5);
    
    for (index = row; index != n - 1; index += 1) {
      for (col = 0; col != mcol; col += 1) {
	QTableItem * it = item(index + 1, col);
	
	takeItem(it);
	setItem(index, col, it);
      }
      setText(index, mcol, text(index + 1, col));
      setText(index, mcol+1, text(index + 1, col));
    }
    
    setNumRows(n - 1);
  }
}

void CppParamsTable::copy_row(int row) {
  int col;
  int mcol = ((dcl) ? 8 : 7);
  
  for (col = 0; col != mcol; col += 1)
    copied[col] = text(row, col);
}

void CppParamsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void CppParamsTable::paste_row(int row) {
  int col;
  int mcol = ((dcl) ? 8 : 7);
  
  for (col = 0; col != mcol; col += 1)
    setText(row, col, copied[col]);
}

void CppParamsTable::move_row(int from, int to) {
  int col;
  int mcol = ((dcl) ? 8 : 7);
  QString save_copied[8];
  
  for (col = 0; col != mcol; col += 1)
    save_copied[col] = copied[col];
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  for (col = 0; col != mcol; col += 1)
    copied[col] = save_copied[col];
}

void CppParamsTable::update_edform() {
  forceUpdateCells();
  
  QString s;
  const char * sep = "";
  
  int n = numRows();
  int index;
  int mcol = ((dcl) ? 6 : 5);
  
  for (index = 0; index != n; index += 1) {
    QString p;
    int col;
    
    for (col = 1; col != mcol; col += 1) {
      if (!text(index, col).isEmpty()) {
	switch (col) {
	case 1:
	case 5:
	  p += text(index, col);
	  break;
	default:
	  p += " " + text(index, col);
	}
      }
    }
    
    if (!text(index, mcol).isEmpty()) {
      if (p.isEmpty() || (text(index, mcol).at(0) == QChar('[')))
	p += text(index, mcol);
      else
	p += " " + text(index, mcol);
    }
    
    p = p.stripWhiteSpace();
    if (! p.isEmpty()) {
      s += sep + p;
      sep = ", ";
    }
  }
  
  QString form = edform->text();
  
  index = form.find("${(}");
  
  form.replace(index + 4, form.find("${)}") - index - 4, s);
  edform->setText(form);
}

//
// CppParamsDialog
//

QSize CppParamsDialog::previous_size;

CppParamsDialog::CppParamsDialog(QWidget * parent, ParamsTable * params,
				 MultiLineEdit * form, bool decl)
    : QDialog(parent, "C++ parameters dialog", TRUE) {
  setCaption(TR("C++ parameters dialog"));

  QVBoxLayout * vbox = new QVBoxLayout(this); 
  
  vbox->setMargin(5);

  tbl = new CppParamsTable(params, form, this, decl);
  vbox->addWidget(tbl);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
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

CppParamsDialog::~CppParamsDialog() {
  previous_size = size();
}

void CppParamsDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}
  

void CppParamsDialog::accept() {
  tbl->update_edform();
  
  QDialog::accept();
}

//
// PhpParamTable
//

// copy/cut/paste
QString PhpParamsTable::copied[6];	// copy/cut/paste

static QStringList PhpTypeRankList;
static QStringList PhpRefList;
static QStringList PhpParamRankList;
static QStringList PhpValueRankList;

PhpParamsTable::PhpParamsTable(QWidget * parent, ParamsTable * p, MultiLineEdit * f)
    : MyTable(0, 6, parent), params(p), edform(f) {
    
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, TR("Name"));
  horizontalHeader()->setLabel(1, "${t<i>}/array");
  horizontalHeader()->setLabel(2, TR("Ref."));
  horizontalHeader()->setLabel(3, "${p<i>}");
  horizontalHeader()->setLabel(4, "${v<i>}");
  horizontalHeader()->setLabel(5, TR("do"));
  setColumnStretchable (0, TRUE);
  setColumnStretchable (1, TRUE);
  adjustColumn(2);
  setColumnStretchable (3, TRUE);
  setColumnStretchable (4, TRUE);
  adjustColumn(5);

  
  QString form = edform->text();
  //the presence of ${(} and ${)} was checked
  int form_index = form.find("${(}") + 4;
  int tbl_index = 0;
    
  while (extract(tbl_index, form_index, form)) {
    setText(tbl_index, 5, QString::null);
    tbl_index += 1;
  }
  
  if (tbl_index == 0)
    insert_row_before(0);
    
  connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	  this, SLOT(button_pressed(int, int, int, const QPoint &)));
  
  if (PhpRefList.isEmpty()) {
    PhpRefList.append("");
    PhpRefList.append("&");
  }
  
  PhpTypeRankList.clear();
  PhpParamRankList.clear();
  PhpValueRankList.clear();
  
  for (int rank = 0; rank != params->numRows(); rank += 1) {
    if (!params->name(rank).isEmpty() || !params->type(rank).isEmpty()) {
      QString s;
      
      s.sprintf("${t%u}", rank);
      PhpTypeRankList.append(s);
      s.sprintf("${p%u}", rank);
      PhpParamRankList.append(s);
      s.sprintf("${v%u}", rank);
      PhpValueRankList.append(s);
    }
  }
  PhpTypeRankList.append(TR("array"));
}

void PhpParamsTable::init_row(int row) {
  setItem(row, 0, new QTableItem(this, QTableItem::Never, QString::null));
  setItem(row, 1, new ComboItem(this, QString::null, PhpTypeRankList));
  setItem(row, 2, new ComboItem(this, QString::null, PhpRefList));
  setItem(row, 3, new ComboItem(this, QString::null, PhpParamRankList));
  setItem(row, 4, new ComboItem(this, QString::null, PhpValueRankList));
  setText(row, 5, QString::null);
}

bool PhpParamsTable::extract(int tblindex, int & strindex, QString s) {
  // s at least contains ${)}
  while (s.at(strindex).isSpace())
    strindex += 1;
  
  int sup = supOf(s, strindex);
  
  if (s.mid(strindex, sup - strindex).stripWhiteSpace().isEmpty())
    return FALSE;
  
  QString t_i;
  QString ptr;
  QString p_i;
  QString v_i;
  int index = s.find("${t", strindex);
  
  if ((index == -1) || (index >= sup)) {
    // no $type
    index = s.find("${p", strindex);
    
    if ((index != -1) && (index < sup)) {
      // no $type, have name
      t_i = s.mid(strindex, index - strindex).stripWhiteSpace();
      
      // extract ptr
      int ir = t_i.find('&');

      if (ir != -1) {
	ptr = t_i.mid(ir).stripWhiteSpace();
	t_i = t_i.left(ir).stripWhiteSpace();
      }
      
      if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
	return FALSE;
      else {
	strindex += 1;
	p_i = s.mid(index, strindex - index);
	v_i = s.mid(strindex);
      }
    }
  }
  else {
    if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
      return FALSE;
  
    strindex += 1;
    t_i = s.mid(index, strindex - index);
    
    index = s.find("${p", strindex);
    
    if ((index != -1) && (index < sup)) {
      // named
      ptr = s.mid(strindex, index - strindex).stripWhiteSpace();
      if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
	return FALSE;
      else {
	strindex += 1;
	p_i = s.mid(index, strindex - index);
	v_i = s.mid(strindex);
      }
    }
  }
  
  if ((v_i.length() >= 5) && (v_i.left(3) == "${v")) {
    if ((strindex = v_i.find('}', 3)) == -1)
      return FALSE;
    else
      v_i = v_i.left(strindex + 1);
  }
  else
    v_i = "";

  setNumRows(tblindex + 1);

  setItem(tblindex, 1, new ComboItem(this, t_i, PhpTypeRankList));
  setItem(tblindex, 2, new ComboItem(this, ptr, PhpRefList));
  setItem(tblindex, 3, new ComboItem(this, p_i, PhpParamRankList));
  setItem(tblindex, 4, new ComboItem(this, v_i, PhpValueRankList));
  
  strindex = (s.at(sup) == QChar(',')) ? sup + 1 : sup;
    
  return TRUE;
}

void PhpParamsTable::setItem(int row, int col, QTableItem * item) {
  QTable::setItem(row, col, item);
  
  if ((col == 1) || (col == 3))
    update_name(row);
}

void PhpParamsTable::setCurrentCell(int row, int col) {
  QTable::setCurrentCell(row, col);
  update_names();
}

void PhpParamsTable::update_names() {
  int n = numRows();
  int row;

  for (row = 0; row != n; row += 1)
    update_name(row);
}

void PhpParamsTable::update_name(int row) {
  bool t_set;
  bool p_set;
  unsigned t_i;
  unsigned p_i;
  
  if (!text(row, 1).isEmpty() &&
      (sscanf((const char *) text(row, 1), "${t%u}", &t_i) == 1))
    t_set = TRUE;
  else
    t_set = FALSE;
  
  if (!text(row, 3).isEmpty() &&
      (sscanf((const char *) text(row, 3), "${p%u}", &p_i) == 1))
    p_set = TRUE;
  else
    p_set = FALSE;
  
  if (t_set) {
    if (p_set)
      QTable::setItem(row, 0,
		      new QTableItem(this, QTableItem::Never,
				     ((t_i == p_i) && (t_i < params->nparams()))
				     ? params->name(t_i) : QString::null));
    else
      QTable::setItem(row, 0,
		      new QTableItem(this, QTableItem::Never,
				     (t_i < params->nparams())
				     ? params->name(t_i) : QString::null));
  }
  else
    QTable::setItem(row, 0,
		    new QTableItem(this, QTableItem::Never,
				   (p_set && (p_i < params->nparams()))
				   ? params->name(p_i) : QString::null));
}

void PhpParamsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 5) {
    char s[16];
    QPopupMenu m;
    
    sprintf(s, "%d", row + 1);
    m.insertItem(TR("param %1", s), -1);
    m.insertSeparator();
    m.insertItem(TR("Insert param before"), 0);
    m.insertItem(TR("Insert param after"), 1);
    m.insertSeparator();
    m.insertItem(TR("Delete param"), 2);
    m.insertSeparator();
    m.insertItem(TR("Copy param"), 3);
    m.insertItem(TR("Cut param"), 4);
    m.insertItem(TR("Paste param"), 5);
    m.insertSeparator();
    
    QPopupMenu mv;
    int rank;
    
    for (rank = 0; rank != numRows(); rank += 1)
      if (rank != row)
	mv.insertItem(QString::number(rank + 1), 10 + rank);
    
    m.insertItem(TR("Move param"), &mv);
    m.insertSeparator();
    
    QPopupMenu rk;
    int t_i;
    int p_i;
    int v_i;
    
    if (text(row, 1).isEmpty() ||
	(sscanf((const char *) text(row, 1), "${t%d}", &t_i) != 1))
      t_i = -1;
    if (text(row, 3).isEmpty() ||
	(sscanf((const char *) text(row, 3), "${p%d}", &p_i) != 1))
      p_i = -1;
    if (text(row, 4).isEmpty() ||
	(sscanf((const char *) text(row, 4), "${v%d}", &v_i) != 1))
      v_i = -1;
    
    for (rank = 0; rank != params->numRows(); rank += 1)
      if ((!params->name(rank).isEmpty() || !params->type(rank).isEmpty()) &&
	  ((rank != t_i) || (rank != p_i)))
	rk.insertItem(QString::number(rank), 100 + rank);
    
    m.insertItem(TR("Set rank <i>"), &rk);
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      break;
    case 1:
      insert_row_after(row);
      break;
    case 2:
      delete_row(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      break;
    case 5:
      paste_row(row);
      break;
    default:
      if (rank >= 100) {
	char s[32];
	
	if (t_i != -1) {
	  sprintf(s, "${t%d}", rank - 100);
	  setItem(row, 1, new ComboItem(this, s, PhpTypeRankList));
	}
	
	if (p_i != -1) {
	  sprintf(s, "${p%d}", rank - 100);
	  setItem(row, 3, new ComboItem(this, s, PhpParamRankList));
	}
	
	if (v_i != -1) {
	  sprintf(s, "${v%d}", rank - 100);
	  setItem(row, 4, new ComboItem(this, s, PhpValueRankList));
	}
      }
      else if (rank >= 10)
	move_row(row, rank - 10);
      break;
    }
  }
}

void PhpParamsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  int col;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    for (col = 0; col != 5; col += 1) {
      QTableItem * it = item(index - 1, col);
      
      takeItem(it);
      setItem(index, col, it);
    }
    setText(index, 5, text(index - 1, 5));
  }

  init_row(row);
 }

void PhpParamsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  int col;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    for (col = 0; col != 5; col += 1) {
      QTableItem * it = item(index - 1, col);
      
      takeItem(it);
      setItem(index, col, it);
    }
    setText(index, 5, text(index - 1, col));
  }

  init_row(row + 1);
 }

void PhpParamsTable::delete_row(int row) {
  int n = numRows();
  int index;
  int col;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    init_row(row);
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      for (col = 0; col != 5; col += 1) {
	QTableItem * it = item(index + 1, col);
	
	takeItem(it);
	setItem(index, col, it);
      }
      setText(index, 5, text(index + 1, col));
    }
    
    setNumRows(n - 1);
  }
}

void PhpParamsTable::copy_row(int row) {
  int col;
  
  for (col = 0; col != 6; col += 1)
    copied[col] = text(row, col);
}

void PhpParamsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void PhpParamsTable::paste_row(int row) {
  int col;
  
  for (col = 0; col != 6; col += 1)
    setText(row, col, copied[col]);
}

void PhpParamsTable::move_row(int from, int to) {
  int col;
  QString save_copied[6];
  
  for (col = 0; col != 6; col += 1)
    save_copied[col] = copied[col];
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  for (col = 0; col != 6; col += 1)
    copied[col] = save_copied[col];
}

void PhpParamsTable::update_edform() {
  forceUpdateCells();
  
  QString s;
  const char * sep = "";
  
  int n = numRows();
  int index;
  
  for (index = 0; index != n; index += 1) {
    QString p;
    int col;
    
    for (col = 1; col != 5; col += 1) {
      if (! text(index, col).isEmpty()) {
	switch (col) {
	case 1:
	case 4:
	  p += text(index, col);
	  break;
	default:
	  p += " " + text(index, col);
	}
      }
    }
    
    p = p.stripWhiteSpace();
    if (! p.isEmpty()) {
      s += sep + p;
      sep = ", ";
    }
  }
  
  QString form = edform->text();
  
  index = form.find("${(}");
  
  form.replace(index + 4, form.find("${)}") - index - 4, s);
  edform->setText(form);
}

//
// PhpParamsDialog
//

QSize PhpParamsDialog::previous_size;

PhpParamsDialog::PhpParamsDialog(QWidget * parent, ParamsTable * params, MultiLineEdit * form)
    : QDialog(parent, "Php parameters dialog", TRUE) {
  setCaption(TR("Php parameters dialog"));

  QVBoxLayout * vbox = new QVBoxLayout(this); 
  
  vbox->setMargin(5);

  tbl = new PhpParamsTable(this, params, form);
  vbox->addWidget(tbl);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
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

PhpParamsDialog::~PhpParamsDialog() {
  previous_size = size();
}

void PhpParamsDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}
  

void PhpParamsDialog::accept() {
  tbl->update_edform();
  
  QDialog::accept();
}

//
// PythonParamTable
//

// copy/cut/paste
QString PythonParamsTable::copied[6];	// copy/cut/paste

static QStringList PythonTypeRankList;
static QStringList PythonModList;
static QStringList PythonParamRankList;
static QStringList PythonValueRankList;

PythonParamsTable::PythonParamsTable(QWidget * parent, ParamsTable * p, MultiLineEdit * f)
    : MyTable(0, 6, parent), params(p), edform(f) {
    
  setSorting(FALSE);
  setSelectionMode(NoSelection);	// single does not work
  setRowMovingEnabled(TRUE);
  horizontalHeader()->setLabel(0, TR("Name"));
  horizontalHeader()->setLabel(1, TR("Modifier"));
  horizontalHeader()->setLabel(2, "${p<i>}");
  horizontalHeader()->setLabel(3, "${t<i>}");
  horizontalHeader()->setLabel(4, "${v<i>}");
  horizontalHeader()->setLabel(5, TR("do"));
  setColumnStretchable (0, TRUE);
  adjustColumn(1);
  setColumnStretchable (2, TRUE);
  setColumnStretchable (3, TRUE);
  setColumnStretchable (4, TRUE);
  adjustColumn(5);

  
  QString form = edform->text();
  //the presence of ${(} and ${)} was checked
  int form_index = form.find("${(}") + 4;
  int tbl_index = 0;
    
  while (extract(tbl_index, form_index, form)) {
    setText(tbl_index, 5, QString::null);
    tbl_index += 1;
  }
  
  if (tbl_index == 0)
    insert_row_before(0);
    
  connect(this, SIGNAL(pressed(int, int, int, const QPoint &)),
	  this, SLOT(button_pressed(int, int, int, const QPoint &)));
  
  if (PythonModList.isEmpty()) {
    PythonModList.append("");
    PythonModList.append("*");
    PythonModList.append("**");
  }
  
  PythonParamRankList.clear();
  PythonTypeRankList.clear();
  PythonValueRankList.clear();
  
  for (int rank = 0; rank != params->numRows(); rank += 1) {
    if (!params->name(rank).isEmpty()) {
      QString s;
      
      s.sprintf("${p%u}", rank);
      PythonParamRankList.append(s);
      
      s.sprintf("${t%u}", rank);
      PythonTypeRankList.append(s);
      
      s.sprintf("${v%u}", rank);
      PythonValueRankList.append(s);
    }
  }
}

void PythonParamsTable::init_row(int row) {
  setItem(row, 0, new QTableItem(this, QTableItem::Never, QString::null));
  setItem(row, 1, new ComboItem(this, QString::null, PythonModList));
  setItem(row, 2, new ComboItem(this, QString::null, PythonParamRankList));
  setItem(row, 3, new ComboItem(this, QString::null, PythonTypeRankList));
  setItem(row, 4, new ComboItem(this, QString::null, PythonValueRankList));
  setText(row, 5, QString::null);
}

bool PythonParamsTable::extract(int tblindex, int & strindex, QString s) {
  // s at least contains ${)}
  while (s.at(strindex).isSpace())
    strindex += 1;
  
  int sup = supOf(s, strindex);
  
  if (s.mid(strindex, sup - strindex).stripWhiteSpace().isEmpty())
    return FALSE;
  
  QString p_i;
  QString t_i;
  QString v_i;
  QString ptr;
  QString m_i;
  int index = s.find("${p", strindex);
    
  if ((index != -1) && (index < sup)) {
    // have name
    m_i = s.mid(strindex, index - strindex).stripWhiteSpace();
      
    // extract modifier
    if (m_i.find("**") != -1)
      m_i = "**";
    else if (m_i.find("*") != -1)
      m_i = "*";
    
    if (((strindex = s.find('}', index + 2)) == -1) || (strindex >= sup))
      return FALSE;
    else {
      strindex += 1;
      p_i = s.mid(index, strindex - index);
      t_i = s.mid(strindex);
    }
  }
  else
    return FALSE;
  
  if ((t_i.length() >= 5) && (t_i.left(3) == "${t")) {
    if ((strindex = t_i.find('}', 3)) == -1)
      return FALSE;
    else {
      strindex += 1;
      v_i = t_i.mid(strindex);
      t_i = t_i.left(strindex);
    }
  }
  else {
    v_i = t_i;
    t_i = "";
  }
  
  if ((v_i.length() >= 5) && (v_i.left(3) == "${v")) {
    if ((strindex = v_i.find('}', 3)) == -1)
      return FALSE;
    else
      v_i = v_i.left(strindex + 1);
  }
  else
    v_i = "";
  
  setNumRows(tblindex + 1);

  setItem(tblindex, 1, new ComboItem(this, m_i, PythonModList));
  setItem(tblindex, 2, new ComboItem(this, p_i, PythonParamRankList));
  setItem(tblindex, 3, new ComboItem(this, t_i, PythonTypeRankList));
  setItem(tblindex, 4, new ComboItem(this, v_i, PythonValueRankList));
  
  strindex = (s.at(sup) == QChar(',')) ? sup + 1 : sup;
    
  return TRUE;
}

void PythonParamsTable::setItem(int row, int col, QTableItem * item) {
  QTable::setItem(row, col, item);
  
  if (col == 2)
    update_name(row);
}

void PythonParamsTable::setCurrentCell(int row, int col) {
  QTable::setCurrentCell(row, col);
  update_names();
}

void PythonParamsTable::update_names() {
  int n = numRows();
  int row;

  for (row = 0; row != n; row += 1)
    update_name(row);
}

void PythonParamsTable::update_name(int row) {
  bool p_set;
  unsigned p_i;
  
  if (!text(row, 2).isEmpty() &&
      (sscanf((const char *) text(row, 2), "${p%u}", &p_i) == 1))
    p_set = TRUE;
  else
    p_set = FALSE;
  
  QTable::setItem(row, 0,
		  new QTableItem(this, QTableItem::Never,
				 (p_set && (p_i < params->nparams()))
				 ? params->name(p_i) : QString::null));
}

void PythonParamsTable::button_pressed(int row, int col, int, const QPoint &) {
  if (col == 5) {
    char s[16];
    QPopupMenu m;
    
    sprintf(s, "%d", row + 1);    
    m.insertItem(TR("param %1", s), -1);
    m.insertSeparator();
    m.insertItem(TR("Insert param before"), 0);
    m.insertItem(TR("Insert param after"), 1);
    m.insertSeparator();
    m.insertItem(TR("Delete param"), 2);
    m.insertSeparator();
    m.insertItem(TR("Copy param"), 3);
    m.insertItem(TR("Cut param"), 4);
    m.insertItem(TR("Paste param"), 5);
    m.insertSeparator();
    
    QPopupMenu mv;
    int rank;
    
    for (rank = 0; rank != numRows(); rank += 1)
      if (rank != row)
	mv.insertItem(QString::number(rank + 1), 10 + rank);
    
    m.insertItem(TR("Move param"), &mv);
    m.insertSeparator();
    
    QPopupMenu rk;
    int p_i;
    int v_i;
    int t_i;
    
    if (text(row, 2).isEmpty() ||
	(sscanf((const char *) text(row, 2), "${p%d}", &p_i) != 1))
      p_i = -1;
    if (text(row, 3).isEmpty() ||
	(sscanf((const char *) text(row, 3), "${t%d}", &t_i) != 1))
      t_i = -1;
    if (text(row, 4).isEmpty() ||
	(sscanf((const char *) text(row, 4), "${v%d}", &v_i) != 1))
      v_i = -1;
    
    for (rank = 0; rank != params->numRows(); rank += 1)
      if (!params->name(rank).isEmpty() && (rank != p_i))
	rk.insertItem(QString::number(rank), 100 + rank);
    
    m.insertItem(TR("Set rank <i>"), &rk);
    
    switch (rank = m.exec(QCursor::pos())) {
    case 0:
      insert_row_before(row);
      break;
    case 1:
      insert_row_after(row);
      break;
    case 2:
      delete_row(row);
      break;
    case 3:
      copy_row(row);
      break;
    case 4:
      cut_row(row);
      break;
    case 5:
      paste_row(row);
      break;
    default:
      if (rank >= 100) {
	char s[32];
	
	if (p_i != -1) {
	  sprintf(s, "${p%d}", rank - 100);
	  setItem(row, 2, new ComboItem(this, s, PythonParamRankList));
	}
	
	if (v_i != -1) {
	  sprintf(s, "${t%d}", rank - 100);
	  setItem(row, 3, new ComboItem(this, s, PythonTypeRankList));
	}
	
	if (v_i != -1) {
	  sprintf(s, "${v%d}", rank - 100);
	  setItem(row, 4, new ComboItem(this, s, PythonValueRankList));
	}
      }
      else if (rank >= 10)
	move_row(row, rank - 10);
      break;
    }
  }
}

void PythonParamsTable::insert_row_before(int row) {
  int n = numRows();
  int index;
  int col;
  
  setNumRows(n + 1);
  
  for (index = n; index != row; index -= 1) {
    for (col = 0; col != 5; col += 1) {
      QTableItem * it = item(index - 1, col);
      
      takeItem(it);
      setItem(index, col, it);
    }
    setText(index, col, text(index - 1, col));
  }

  init_row(row);
}

void PythonParamsTable::insert_row_after(int row) {
  int n = numRows();
  int index;
  int col;
  
  setNumRows(n + 1);
  
  for (index = n; index > row + 1; index -= 1) {
    for (col = 0; col != 5; col += 1) {
      QTableItem * it = item(index - 1, col);
      
      takeItem(it);
      setItem(index, col, it);
    }
    setText(index, col, text(index - 1, col));
  }

  init_row(row + 1);
 }

void PythonParamsTable::delete_row(int row) {
  int n = numRows();
  int index;
  int col;

  clearCellWidget(row, 1);
    
  if (row == (n - 1)) {
    // the last line : empty it
    init_row(row);
  }
  else {
    for (index = row; index != n - 1; index += 1) {
      for (col = 0; col != 5; col += 1) {
	QTableItem * it = item(index + 1, col);
	
	takeItem(it);
	setItem(index, col, it);
      }
      setText(index, col, text(index + 1, col));
    }
    
    setNumRows(n - 1);
  }
}

void PythonParamsTable::copy_row(int row) {
  int col;
  
  for (col = 0; col != 6; col += 1)
    copied[col] = text(row, col);
}

void PythonParamsTable::cut_row(int row) {
  copy_row(row);
  delete_row(row);
}

void PythonParamsTable::paste_row(int row) {
  int col;
  
  for (col = 0; col != 6; col += 1)
    setText(row, col, copied[col]);
}

void PythonParamsTable::move_row(int from, int to) {
  int col;
  QString save_copied[6];
  
  for (col = 0; col != 6; col += 1)
    save_copied[col] = copied[col];
  
  cut_row(from);
  if (to > from)
    insert_row_after(to - 1);
  else
    insert_row_before(to);
  paste_row(to);
  
  for (col = 0; col != 6; col += 1)
    copied[col] = save_copied[col];
}

void PythonParamsTable::update_edform() {
  forceUpdateCells();
  
  QString s;
  const char * sep = "";
  
  int n = numRows();
  int index;
  
  for (index = 0; index != n; index += 1) {
    QString p;
    int col;
    
    for (col = 1; col != 5; col += 1)
      if (!text(index, col).isEmpty())
	p += text(index, col);
    
    p = p.stripWhiteSpace();
    if (! p.isEmpty()) {
      s += sep + p;
      sep = ", ";
    }
  }
  
  QString form = edform->text();
  
  index = form.find("${(}");
  
  form.replace(index + 4, form.find("${)}") - index - 4, s);
  edform->setText(form);
}

//
// PythonParamsDialog
//

QSize PythonParamsDialog::previous_size;

PythonParamsDialog::PythonParamsDialog(QWidget * parent, ParamsTable * params, MultiLineEdit * form)
    : QDialog(parent, "Python parameters dialog", TRUE) {
  setCaption(TR("Python parameters dialog"));

  QVBoxLayout * vbox = new QVBoxLayout(this); 
  
  vbox->setMargin(5);

  tbl = new PythonParamsTable(this, params, form);
  vbox->addWidget(tbl);
  
  QHBoxLayout * hbox = new QHBoxLayout(vbox); 
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

PythonParamsDialog::~PythonParamsDialog() {
  previous_size = size();
}

void PythonParamsDialog::polish() {
  QDialog::polish();
  UmlDesktop::limitsize_center(this, previous_size, 0.8, 0.8);
}
  

void PythonParamsDialog::accept() {
  tbl->update_edform();
  
  QDialog::accept();
}
