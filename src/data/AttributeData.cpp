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

#include "AttributeData.h"
#include "BrowserClass.h"
#include "ClassData.h"
#include "ClassDialog.h"
#include "BrowserAttribute.h"
#include "BrowserOperation.h"
#include "AttributeDialog.h"
#include "GenerationSettings.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"
#include "translate.h"

AttributeData::AttributeData()
    : is_deleted(FALSE),
      isa_class_attribute(FALSE), isa_volatile_attribute(FALSE),
      isa_const_attribute(FALSE),
      is_derived(FALSE),
      is_derivedunion(FALSE),
      is_ordered(FALSE),
      is_unique(FALSE),
      cpp_mutable(FALSE),
      java_transient(FALSE),
      uml_visibility(UmlDefaultVisibility),
      cpp_visibility(UmlDefaultVisibility), idl_case(0) {
}

AttributeData::AttributeData(const AttributeData * model, BrowserNode * bn)
    : ClassMemberData(model),
      multiplicity(model->multiplicity),
      init_value(model->init_value), 
      constraint(model->constraint), 
      is_deleted(FALSE), isa_class_attribute(model->isa_class_attribute),
      isa_volatile_attribute(model->isa_volatile_attribute),
      isa_const_attribute(model->isa_const_attribute),
      is_derived(model->is_derived),
      is_derivedunion(model->is_derivedunion),
      is_ordered(model->is_ordered),
      is_unique(model->is_unique),
      cpp_mutable(model->cpp_mutable),
      java_transient(model->java_transient),
      uml_visibility(model->uml_visibility),
      cpp_visibility(model->cpp_visibility), cpp_decl(model->cpp_decl), 
      java_decl(model->java_decl), java_annotation(model->java_annotation),
      php_decl(model->php_decl),
      python_decl(model->python_decl), 
      idl_decl(model->idl_decl) {
  browser_node = bn;
  idl_case = 0;	// set_idlcase look at current value
  set_type(model->type);
  set_idlcase(model->idl_case, model->idl_explicit_case);
}

AttributeData::~AttributeData() {
}

bool AttributeData::deletedp() const {
  return is_deleted;
}

void AttributeData::set_deletedp(bool y) {
  is_deleted = y;
}

void AttributeData::set_browser_node(BrowserAttribute * a, bool update,
				     bool enum_item) {
  BasicData::set_browser_node(a);
  
  if (update) {
    if (enum_item)
      type.type = (BrowserClass *) a->parent();
    
    if (uml_visibility == UmlDefaultVisibility)
      uml_visibility = 
	((BrowserNode *) browser_node->parent())->get_visibility(UmlAttribute);
  
    QString cl_stereotype =
      ((ClassData *) (((BrowserClass *) browser_node->parent())->get_data()))->get_stereotype();
  
    if (GenerationSettings::cpp_get_default_defs()) {
      if (ClassDialog::cpp_stereotype(cl_stereotype) != "enum")
	cpp_decl = GenerationSettings::cpp_default_attr_decl(multiplicity);
      else
	cpp_decl = (enum_item) ? GenerationSettings::cpp_default_enum_item_decl() : "";
    }
    
    if (GenerationSettings::java_get_default_defs()) {
      bool enump = ClassDialog::java_stereotype(cl_stereotype) == "enum";
      
      if (!enum_item) {
	java_decl = GenerationSettings::java_default_attr_decl(multiplicity);
	if (enump)
	  set_stereotype("attribute");
      }
      else 
	java_decl = (enump)
	  ? GenerationSettings::java_default_enum_item_decl()
	  : GenerationSettings::java_default_enum_pattern_item_decl();
    }
    
    if (GenerationSettings::php_get_default_defs()) {
      php_decl = (enum_item)
	? GenerationSettings::php_default_enum_item_decl()
	: GenerationSettings::php_default_attr_decl();
    }
    
    if (GenerationSettings::python_get_default_defs()) {
      python_decl = (enum_item)
	? GenerationSettings::python_default_enum_item_decl()
	: GenerationSettings::python_default_attr_decl(multiplicity);
    }
    
    QString idl_st = ClassDialog::idl_stereotype(cl_stereotype);
    
    if (GenerationSettings::idl_get_default_defs()) {
      if (idl_st == "enum")
	idl_decl =  (enum_item) ? GenerationSettings::idl_default_enum_item_decl() :  "";
      else if (idl_st == "union")
	idl_decl = GenerationSettings::idl_default_union_item_decl(multiplicity);
      else if (idl_st == "valuetype")
	idl_decl = GenerationSettings::idl_default_valuetype_attr_decl(multiplicity);
      else
	idl_decl = GenerationSettings::idl_default_attr_decl(multiplicity);
    }
  }
}

QString AttributeData::definition(bool full, bool with_kind) const {
  QString r;

  if (with_kind)
    r = "[" + browser_node->get_stype() + "] ";

  if (! full)
    r += browser_node->get_name();
  else
    r += ((const char *) browser_node->get_name())
      + QString(" : ") + ((const char *) type.get_type());

  return r;
}

QString AttributeData::definition(bool full, bool mult, bool init,
				  bool modif, DrawingLanguage language,
				  ShowContextMode mode) const {
  switch (language) {
  case UmlView:
    {
      QString r = ((const char *) browser_node->get_name());
      
      if (is_derived)
	r = "/" + r;
      
      if (full) {
	r += " : " + type.get_type(mode);
	
	if (mult && !multiplicity.isEmpty())
	  r += " [" + QString((const char *) multiplicity) + "]";
	
	if (init && !init_value.isEmpty()) {
	  if (*init_value != '=')
	    r += " = ";
	  r += (const char *) init_value;
	}
      }
      
      if (modif) {
	const char * sep = " {";
	
	if (isa_const_attribute) {
	  r += QString(sep) + "readOnly";
	  sep = ",";
	}
	if (is_derived && is_derivedunion) {
	  r += QString(sep) + "union";
	  sep = ",";
	}
	if (is_ordered) {
	  r += QString(sep) + "ordered";
	  sep = ",";
	}
	if (is_unique) {
	  r += QString(sep) + "unique";
	  sep = ",";
	}
	if (*sep == ',')
	  r += "}";
      }
      
      return r;
    }
  case CppView:
    if (full)
      return AttributeDialog::cpp_decl((BrowserAttribute *) browser_node,
				       init, mode);
    else if (!cpp_decl.isEmpty())
      return definition(FALSE, FALSE);
    else
      return QString::null;
  case JavaView:
    if (full)
      return AttributeDialog::java_decl((BrowserAttribute *) browser_node,
					init, mode);
    else if (!java_decl.isEmpty())
      return definition(FALSE, FALSE);
    else
      return QString::null;
  case PhpView:
    if (full)
      return AttributeDialog::php_decl((BrowserAttribute *) browser_node,
				       init, mode);
    else if (!php_decl.isEmpty())
      return definition(FALSE, FALSE);
    else
      return QString::null;
  case PythonView:
    if (full)
      return AttributeDialog::python_decl((BrowserAttribute *) browser_node,
					  init, mode);
    else if (!python_decl.isEmpty())
      return definition(FALSE, FALSE);
    else
      return QString::null;
  default:
    if (full)
      return AttributeDialog::idl_decl((BrowserAttribute *) browser_node, mode);
    else if (!idl_decl.isEmpty())
      return definition(FALSE, FALSE);
    else
      return QString::null;
  }	
}

bool AttributeData::decldefbody_contain(const QString & s, bool cs,
					BrowserNode *) {
  return ((QString(get_cppdecl()).find(s, 0, cs) != -1) ||
	  (QString(get_javadecl()).find(s, 0, cs) != -1) ||
	  (QString(get_phpdecl()).find(s, 0, cs) != -1) ||
	  (QString(get_pythondecl()).find(s, 0, cs) != -1) ||
	  (QString(get_idldecl()).find(s, 0, cs) != -1));
}

UmlVisibility AttributeData::get_visibility(BrowserNode *) {
  return uml_visibility;
}

void AttributeData::set_visibility(UmlVisibility v) {
  uml_visibility = v;
}

void AttributeData::set_type(const AType & t) {
  if (type.type != t.type) {
    if (type.type != 0)
      disconnect(type.type->get_data(), SIGNAL(deleted()),
		 this, SLOT(on_delete()));
    if ((type.type = t.type) != 0) {
      connect(type.type->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete()));
    }
  }
    
  type.explicit_type = t.explicit_type;
}

void AttributeData::on_delete() {
  if (type.type && type.type->deletedp()) {
    type.explicit_type = type.type->get_name();
    type.type = 0;
  }
  
  modified();
}

const char * AttributeData::get_idlcase() const {
  return (idl_case != 0)
    ? ((const char *) idl_case->get_name())
    : ((const char *) idl_explicit_case);
}

void AttributeData::set_idlcase(BrowserAttribute * a, const char * e) {
  if (idl_case != a) {
    if (idl_case != 0)
      disconnect(idl_case->get_data(), SIGNAL(deleted()),
		 this, SLOT(on_delete_idlcase()));
    if ((idl_case = a) != 0)
      connect(idl_case->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete_idlcase()));
  }
  
  idl_explicit_case = e;
}

void AttributeData::on_delete_idlcase() {
  if (idl_case && idl_case->deletedp()) {
    idl_explicit_case = idl_case->get_name();
    idl_case = 0;
  }
  
  modified();
}

void AttributeData::edit(bool new_st_attr) {
  setName(browser_node->get_name());
  
  (new AttributeDialog(this, new_st_attr))->show();
}

//

void AttributeData::replace(BrowserClass * old, BrowserClass * nw) {
  if (type.type == old) {
    AType t;
    
    t.type = nw;
    set_type(t);
  }
}

//

void AttributeData::send_uml_def(ToolCom * com, BrowserNode * bn, 
				const QString & comment) {
  int api = com->api_format();
  
  BasicData::send_uml_def(com, bn, comment);
  com->write_bool(isa_class_attribute);
  if (api >= 13)
    com->write_char(isa_volatile_attribute);
  com->write_char(((api >= 23) ||
		   (uml_visibility != UmlPackageVisibility))
		  ? uml_visibility : UmlPublic);
  
  if (api >= 30)
    com->write_string(constraint);
  
  type.send_def(com);

  if (api >= 32)
    com->write_string(multiplicity);

  com->write_string(init_value);
  com->write_bool(isa_const_attribute);
  
  if (api > 41) {
    com->write_bool(is_derived);
    com->write_bool(is_derivedunion);
    com->write_bool(is_ordered);
    com->write_bool(is_unique);
  }
  
  if (((BrowserAttribute *) browser_node)->get_get_oper() != 0)
    ((BrowserAttribute *) browser_node)->get_get_oper()->write_id(com);
  else
    com->write_id(0);
    
  if (((BrowserAttribute *) browser_node)->get_set_oper() != 0)
    ((BrowserAttribute *) browser_node)->get_set_oper()->write_id(com);
  else
    com->write_id(0);
}

void AttributeData::send_cpp_def(ToolCom * com) {
  com->write_string(cpp_decl);
  if (com->api_format() >= 23)
    com->write_char(cpp_visibility);
  else {
    switch(cpp_visibility) {
    case UmlPackageVisibility:
      com->write_char(UmlPublic);
      break;
    case UmlDefaultVisibility:
      com->write_char(UmlDefaultVisibility - 1);
      break;
    default:
      com->write_char(cpp_visibility);
    }
  }
  if (com->api_format() < 13)
    com->write_char(isa_volatile_attribute);
  com->write_char(cpp_mutable);
}

void AttributeData::send_java_def(ToolCom * com) {
  com->write_string(java_decl);
  if (com->api_format() >= 21)
    com->write_string(java_annotation);
  com->write_bool(java_transient);
  if ((com->api_format() >= 19) && (com->api_format() < 21))
    com->write_string(java_annotation);
}

void AttributeData::send_php_def(ToolCom * com) {
  com->write_string(php_decl);
}

void AttributeData::send_python_def(ToolCom * com) {
  com->write_string(python_decl);
}

void AttributeData::send_idl_def(ToolCom * com) {
  com->write_string(idl_decl);
  if (idl_case == 0) {
    com->write_id(0);
    com->write_string(idl_explicit_case);
  }
  else
    idl_case->write_id(com);  
}

bool AttributeData::tool_cmd(ToolCom * com, const char * args,
			    BrowserNode * bn,
			    const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setIsClassMemberCmd:
	isa_class_attribute = (*args != 0);
	break;
      case setIsVolatileCmd:
	isa_volatile_attribute = (*args != 0);
	break;
      case setVisibilityCmd:
	{
	  UmlVisibility v;
	  
	  if (! com->get_visibility(v, args)) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
	  else
	    uml_visibility = v;
	}
	break;
      case setMultiplicityCmd:
	multiplicity = args;
	break;
      case setConstraintCmd:
	constraint = args;
	break;
      case setCppDeclCmd:
	cpp_decl = args;
	break;
      case setJavaDeclCmd:
	java_decl = args;
	break;
      case setJavaAnnotationCmd:
	{
	  QCString s = args;

	  s = s.stripWhiteSpace();
	  if (! s.isEmpty())
	    s += '\n';
	  java_annotation = s;
	}
	break;
      case setPhpDeclCmd:
	php_decl = args;
	break;
      case setPythonDeclCmd:
	python_decl = args;
	break;
      case setIdlDeclCmd:
	idl_decl = args;
	break;
      case setCppVisibilityCmd:
	{
	  UmlVisibility v;
	  
	  if (! com->get_extended_visibility(v, args)) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
	  else
	    cpp_visibility = v;
	}
	break;
      case setIsReadOnlyCmd:
	isa_const_attribute = (*args != 0);
	break;
      case setDefaultValueCmd:
	init_value = args;
	break;
      case setTypeCmd:
	{
	  AType t;
	  
	  com->get_type(t, args);
	  set_type(t);
	}
	break;
      case setIsCppMutableCmd:
	cpp_mutable = (*args != 0);
	break;
      case setIsJavaTransientCmd:
	java_transient = (*args != 0);
	break;
      case setIdlCaseCmd:
	{
	  BrowserAttribute * at = (BrowserAttribute *) com->get_id(args);
	  
	  set_idlcase(at, args);
	}
	break;
      case setDerivedCmd:
	switch (*args) {
	case 0:
	  is_derived = is_derivedunion = FALSE;
	  break;
	case 1:
	  is_derived = TRUE;
	  is_derivedunion = FALSE;
	  break;
	case 3:
	  is_derived = is_derivedunion = TRUE;
	  break;
	default:
	  // derived union but non derived
	  com->write_ack(FALSE);
	  return TRUE;
	}
	break;
      case setOrderingCmd:
	is_ordered = (*args != 0);
	break;
      case setUniqueCmd:
	is_unique = (*args != 0);
	break;
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
      
      // ok case
      bn->modified();
      bn->package_modified();
      modified();
      com->write_ack(TRUE);
    }
    return TRUE;
  }
  else
    return BasicData::tool_cmd(com, args, bn, comment);
}

//

void AttributeData::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  if (isa_class_attribute)
    st << "class_attribute ";
  if (isa_volatile_attribute)
    st << "volatile ";
  if (isa_const_attribute)
    st << "const_attribute ";
  if (is_derived)
    st << ((is_derivedunion) ? "derivedunion " : "derived ");
  if (is_ordered)
    st << "ordered ";
  if (is_unique)
    st << "unique ";

  st << stringify(uml_visibility);
  type.save(st, warning, " type ", " explicit_type ");
  if (! multiplicity.isEmpty()) {
    nl_indent(st);
    st << "multiplicity ";
    save_string(multiplicity, st);
  }
  if (!init_value.isEmpty()) {
    nl_indent(st);
    st << "init_value ";
    save_string(init_value, st);
  }
  if (!constraint.isEmpty()) {
    nl_indent(st);
    st << "constraint ";
    save_string(constraint, st);
  }
  
  BasicData::save(st, warning);
  
  nl_indent(st);
  if (cpp_mutable)
    st << "cpp_mutable ";
  if (cpp_visibility != UmlDefaultVisibility)
    st << "cpp_visibility " << stringify(cpp_visibility) << " ";
  st << "cpp_decl ";
  save_string(cpp_decl, st);
  
  nl_indent(st);
  if (java_transient)
    st << "transient ";
  st << "java_decl ";
  save_string(java_decl, st);  
  if (! java_annotation.isEmpty()) {
    nl_indent(st);
    st << "java_annotation ";
    save_string(java_annotation, st);
  }
  
  nl_indent(st);
  st << "php_decl ";
  save_string(php_decl, st);  
  
  nl_indent(st);
  st << "python_decl ";
  save_string(python_decl, st);  
  
  nl_indent(st);
  if (idl_case != 0) {
    st << "idl_case ";
    idl_case->save(st, TRUE, warning);
    nl_indent(st);
  }
  else if (! idl_explicit_case.isEmpty()) {
    st << "idl_explicit_case ";
    save_string(idl_explicit_case, st);
    nl_indent(st);
  }
  st << "idl_decl ";
  save_string(idl_decl, st);
}

void AttributeData::read(char * & st, char * & k) {
  if (!strcmp(k, "class_attribute") ||
      !strcmp(k, "class_attribut")) {
    isa_class_attribute = TRUE;
    k = read_keyword(st);
  }
  else
    isa_class_attribute = FALSE;
  
  if (!strcmp(k, "volatile")) {
    isa_volatile_attribute = TRUE;
    k = read_keyword(st);
  }
  else
    isa_volatile_attribute = FALSE;
  
  if (!strcmp(k, "const_attribute") ||
      !strcmp(k, "const_attribut")) {
    isa_const_attribute = TRUE;
    k = read_keyword(st);
  }
  else
    isa_const_attribute = FALSE;
  
  if (!strcmp(k, "derivedunion")) {
    is_derived = TRUE;
    is_derivedunion = TRUE;
    k = read_keyword(st);
  }
  else if (!strcmp(k, "derived")) {
    is_derived = TRUE;
    is_derivedunion = FALSE;
    k = read_keyword(st);
  }
  else
    is_derived = is_derivedunion = FALSE;
  
  if (!strcmp(k, "ordered")) {
    is_ordered = TRUE;
    k = read_keyword(st);
  }
  else
    is_ordered = FALSE;
  
  if (!strcmp(k, "unique")) {
    is_unique = TRUE;
    k = read_keyword(st);
  }
  else
    is_unique = FALSE;
  
  uml_visibility = ::visibility(k);
  
  k = read_keyword(st);

  AType t;
  
  t.read(st, "type", "explicit_type", k);
  set_type(t);
  
  k = read_keyword(st);
  if (!strcmp(k, "multiplicity")) {
    multiplicity = read_string(st);
    k = read_keyword(st);
  }
  else
    multiplicity = 0;
  
  if (!strcmp(k, "init_value")) {
    init_value = read_string(st);
    k = read_keyword(st);
  }
  else
    init_value = QString::null;
  
  if (!strcmp(k, "constraint")) {
    constraint = read_string(st);
    k = read_keyword(st);
  }
  else
    constraint = QString::null;
  
  BasicData::read(st, k);	// updates k
  
  if (!strcmp(k, "cpp_volatile")) {
    // old version
    isa_volatile_attribute = TRUE;
    k = read_keyword(st);
  }

  if (!strcmp(k, "cpp_mutable")) {
    cpp_mutable = TRUE;
    k = read_keyword(st);
  }
  else
    cpp_mutable = FALSE;

  if (!strcmp(k, "cpp_visibility")) {
    cpp_visibility = ::visibility(read_keyword(st));
    k = read_keyword(st);
  }
  else
    cpp_visibility = UmlDefaultVisibility;

  if (!strcmp(k, "cpp_decl")) {
    cpp_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    cpp_decl = QString::null;
  
  if (!strcmp(k, "transient")) {
    java_transient = TRUE;
    k = read_keyword(st);
  }
  else
    java_transient = FALSE;
    
  if (!strcmp(k, "java_decl")) {
    java_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    java_decl = QString::null;
  
  if (!strcmp(k, "java_annotation")) {
    java_annotation = read_string(st);
    k = read_keyword(st);
  }
  else
    java_annotation = QString::null;
    
  if (!strcmp(k, "php_decl")) {
    php_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    php_decl = QString::null;
    
  if (!strcmp(k, "python_decl")) {
    python_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    python_decl = QString::null;
    
  if (!strcmp(k, "idl_case")) {
    set_idlcase(BrowserAttribute::read_ref(st), "");
    k = read_keyword(st);
  }
  else if (!strcmp(k, "idl_explicit_case")) {
    set_idlcase(0, read_string(st));
    k = read_keyword(st);
  }
  else
    set_idlcase(0, "");
    
  if (!strcmp(k, "idl_decl")) {
    idl_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    idl_decl = QString::null;
}
