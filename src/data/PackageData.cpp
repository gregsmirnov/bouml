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

#include "BrowserPackage.h"
#include "PackageData.h"
#include "PackageDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

PackageData::PackageData() {
}

PackageData::PackageData(PackageData * model)
    : SimpleData(model) {
  cpp_src_dir = model->cpp_src_dir;
  cpp_h_dir = model->cpp_h_dir;
  java_dir = model->java_dir;
  php_dir = model->php_dir;
  python_dir = model->python_dir;
  idl_dir = model->idl_dir;

  cpp_namespace = model->cpp_namespace;
  java_package = model->java_package;
  php_namespace = model->php_namespace;
  python_package = model->python_package;
  idl_module = model->idl_module;
}

PackageData::~PackageData() {
}

void PackageData::edit() {
  setName(browser_node->get_name());
    
  (new PackageDialog(this))->show();
}

//

void PackageData::send_cpp_def(ToolCom * com) {
  com->write_string(cpp_src_dir);
  com->write_string(cpp_h_dir);
  com->write_string(cpp_namespace);
}

void PackageData::send_java_def(ToolCom * com) {
  com->write_string(java_dir);
  com->write_string(java_package);
}

void PackageData::send_php_def(ToolCom * com) {
  com->write_string(php_dir);
  if (com->api_format() >= 55)
    com->write_string(php_namespace);
}

void PackageData::send_python_def(ToolCom * com) {
  com->write_string(python_dir);
  com->write_string(python_package);
}

void PackageData::send_idl_def(ToolCom * com) {
  com->write_string(idl_dir);
  com->write_string(idl_module);
}

//

bool PackageData::tool_cmd(ToolCom * com, const char * args,
			   BrowserNode * bn,
			   const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setCppSrcDirCmd:
	cpp_src_dir = args;
	break;
      case setCppHDirCmd:
	cpp_h_dir = args;
	break;
      case setCppNamespaceCmd:
	cpp_namespace = args;
	break;
      case setJavaDirCmd:
	java_dir = args;
	break;
      case setJavaPackageCmd:
	java_package = args;
	break;
      case setPhpDirCmd:
	php_dir = args;
	break;
      case setPhpNamespaceCmd:
	php_namespace = args;
	break;
      case setPythonDirCmd:
	python_dir = args;
	break;
      case setPythonPackageCmd:
	python_package = args;
	break;
      case setIdlDirCmd:
	idl_dir = args;
	break;
      case setIdlModuleCmd:
	idl_module = args;
	break;
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
      
      // ok case
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

void PackageData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);

  if (!cpp_h_dir.isEmpty()) {
    nl_indent(st);
    st << "cpp_h_dir ";
    save_string(cpp_h_dir, st);
  }
  if (!cpp_src_dir.isEmpty()) {
    nl_indent(st);
    st << "cpp_src_dir ";
    save_string(cpp_src_dir, st);
  }
  if (!cpp_namespace.isEmpty()) {
    nl_indent(st);
    st << "cpp_namespace ";
    save_string(cpp_namespace, st);
  }
  if (!java_dir.isEmpty()) {
    nl_indent(st);
    st << "java_dir ";
    save_string(java_dir, st);
  }
  if (!java_package.isEmpty()) {
    nl_indent(st);
    st << "java_package ";
    save_string(java_package, st);
  }
  if (!php_dir.isEmpty()) {
    nl_indent(st);
    st << "php_dir ";
    save_string(php_dir, st);
  }
  if (!php_namespace.isEmpty()) {
    nl_indent(st);
    st << "php_namespace ";
    save_string(php_namespace, st);
  }
  if (!python_dir.isEmpty()) {
    nl_indent(st);
    st << "python_dir ";
    save_string(python_dir, st);
  }
  if (!python_package.isEmpty()) {
    nl_indent(st);
    st << "python_package ";
    save_string(python_package, st);
  }
  if (!idl_dir.isEmpty()) {
    nl_indent(st);
    st << "idl_dir ";
    save_string(idl_dir, st);
  }
  if (!idl_module.isEmpty()) {
    nl_indent(st);
    st << "idl_module ";
    save_string(idl_module, st);
  }
}

void PackageData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k

  if (!strcmp(k, "cpp_h_dir")) {
    cpp_h_dir = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "cpp_src_dir")) {
    cpp_src_dir = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "cpp_namespace")) {
    cpp_namespace = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "java_dir")) {
    java_dir = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "java_package")) {
    java_package = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "php_dir")) {
    php_dir = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "php_namespace")) {
    php_namespace = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "python_dir")) {
    python_dir = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "python_package")) {
    python_package = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "idl_dir")) {
    idl_dir = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "idl_module")) {
    idl_module = read_string(st);
    k = read_keyword(st);
  }
}
