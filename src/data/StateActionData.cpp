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

#include "BrowserStateAction.h"
#include "StateActionData.h"
#include "StateActionDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

StateActionData::StateActionData() {
}

StateActionData::StateActionData(StateActionData * model, BrowserNode * bn)
    : SimpleData(model) {
  browser_node = bn;
  uml = model->uml;
  cpp = model->cpp;
  java = model->java;
  stereotype = model->stereotype;
}

StateActionData::~StateActionData() {
}

const char * StateActionData::get_action(DrawingLanguage ln) const {
  switch (ln) {
  case CppView:
    return cpp;
  case JavaView:
    return java;
  default:
    return uml;
  }
}

void StateActionData::edit() {
  (new StateActionDialog(this))->show();
}

//
  
void StateActionData::send_uml_def(ToolCom * com, BrowserNode * bn,
				   const QString & comment) {
  SimpleData::send_uml_def(com, bn, comment);
  com->write_string(uml);
}

void StateActionData::send_cpp_def(ToolCom * com) {
  com->write_string(cpp);
}

void StateActionData::send_java_def(ToolCom * com) {
  com->write_string(java);
}

bool StateActionData::tool_cmd(ToolCom * com, const char * args,
			       BrowserNode * bn,
			       const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setDefCmd:
	uml = args;
	break;
      case setCppDeclCmd:
	cpp = args;
	break;
      case setJavaDeclCmd:
	java = args;
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

void StateActionData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  if (! uml.isEmpty()) {
    nl_indent(st);
    st << "uml ";
    save_string(uml, st);
  }
  if (! cpp.isEmpty()) {
    nl_indent(st);
    st << "cpp ";
    save_string(cpp, st);
  }
  if (! java.isEmpty()) {
    nl_indent(st);
    st << "java ";
    save_string(java, st);
  }
}

void StateActionData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  if (!strcmp(k, "uml")) {
    uml = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "cpp")) {
    cpp = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "java")) {
    java = read_string(st);
    k = read_keyword(st);
  }
}
