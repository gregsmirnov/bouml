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





#include "ExtraMemberData.h"
#include "BrowserClass.h"
#include "ExtraMemberData.h"
#include "ExtraMemberDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

ExtraMemberData::ExtraMemberData()
    : is_deleted(FALSE), cpp_inline(FALSE) {
}

ExtraMemberData::ExtraMemberData(const ExtraMemberData * model, BrowserNode * bn)
    : BasicData(model), is_deleted(FALSE),
      cpp_inline(model->cpp_inline), 
      cpp_decl(model->cpp_decl), cpp_def(model->cpp_def), 
      java_decl(model->java_decl), php_decl(model->php_decl), 
      python_decl(model->python_decl), 
      idl_decl(model->idl_decl) {
  browser_node = bn;
}

ExtraMemberData::~ExtraMemberData() {
}

bool ExtraMemberData::deletedp() const {
  return is_deleted;
}

void ExtraMemberData::set_deletedp(bool y) {
  is_deleted = y;
}

QString ExtraMemberData::definition(bool, bool with_kind) const {
  QString name = browser_node->get_name();

  if (with_kind)
    return "[" + browser_node->get_stype() + "] " + name;
  else if (! name.isEmpty())
    return name;
  else
    return browser_node->get_stype();
}

void ExtraMemberData::edit() {
  setName(browser_node->get_name());
  
  (new ExtraMemberDialog(this))->show();
}

//

void ExtraMemberData::send_cpp_def(ToolCom * com) {
  com->write_string(cpp_decl);
  com->write_string(cpp_def);
  com->write_bool(cpp_inline);
}

void ExtraMemberData::send_java_def(ToolCom * com) {
  com->write_string(java_decl);
}

void ExtraMemberData::send_php_def(ToolCom * com) {
  com->write_string(php_decl);
}

void ExtraMemberData::send_python_def(ToolCom * com) {
  com->write_string(python_decl);
}

void ExtraMemberData::send_idl_def(ToolCom * com) {
  com->write_string(idl_decl);
}

bool ExtraMemberData::tool_cmd(ToolCom * com, const char * args,
			       BrowserNode * bn,
			       const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setCppDeclCmd:
	cpp_decl = args;
	break;
      case setCppDefCmd:
	cpp_def = args;
	break;
      case setIsCppInlineCmd:
	cpp_inline = (*args != 0);
	break;
      case setJavaDeclCmd:
	java_decl = args;
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
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
  
      // ok case
      bn->modified();
      bn->package_modified();
      modified();
      com->write_ack(TRUE);
    }
  }
  else
    return BasicData::tool_cmd(com, args, bn, comment);
  
  return TRUE;
}

//

void ExtraMemberData::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  BasicData::save(st, warning);
  
  nl_indent(st);
  st << "cpp ";
  save_string(cpp_decl, st);
  st << ' ';
  save_string(cpp_def, st);
  if (cpp_inline) 
    st << " inline";
  nl_indent(st);
  st << "java ";
  save_string(java_decl, st);
  nl_indent(st);
  st << "php ";
  save_string(php_decl, st);
  nl_indent(st);
  st << "python ";
  save_string(python_decl, st);
  nl_indent(st);
  st << "idl ";
  save_string(idl_decl, st);
}

void ExtraMemberData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  
  if (!strcmp(k, "cpp")) {
    cpp_decl = read_string(st);
    cpp_def = read_string(st);
    k = read_keyword(st);
    if (!strcmp(k, "inline")) {
      cpp_inline = TRUE;
      k = read_keyword(st);
    }
    else
      cpp_inline = FALSE;
  }
  else {
    cpp_decl = QString::null;
    cpp_def = QString::null;
    cpp_inline = FALSE;
  }
  
  if (!strcmp(k, "java")) {
    java_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    java_decl = QString::null;
    
  if (!strcmp(k, "php")) {
    php_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    php_decl = QString::null;
    
  if (!strcmp(k, "python")) {
    python_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    python_decl = QString::null;
    
  if (!strcmp(k, "idl")) {
    idl_decl = read_string(st);
    k = read_keyword(st);
  }
  else
    idl_decl = QString::null;
}
