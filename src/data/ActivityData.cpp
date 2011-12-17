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

#include "BrowserActivity.h"
#include "ActivityData.h"
#include "ActivityDialog.h"
#include "BrowserOperation.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

ActivityData::ActivityData()
    : read_only(FALSE), single_execution(FALSE), is_active(FALSE), specification(0) {
}

ActivityData::ActivityData(ActivityData * model, BrowserNode * bn)
    : SimpleData(model), specification(0) {
  browser_node = bn;
  uml_condition = model->uml_condition;
  cpp_condition = model->cpp_condition;
  java_condition = model->java_condition;
  constraint = model->constraint;
  read_only = model->read_only;
  single_execution = model->single_execution;
  is_active = model->is_active;
  set_specification(model->specification);
}

ActivityData::~ActivityData() {
}

QString ActivityData::get_precond(DrawingLanguage ln) const {
  switch (ln) {
  case CppView:
    return cpp_condition.first;
  case JavaView:
    return java_condition.first;
  default:
    return uml_condition.first;
  }
}

QString ActivityData::get_postcond(DrawingLanguage ln) const {
  switch (ln) {
  case CppView:
    return cpp_condition.second;
  case JavaView:
    return java_condition.second;
  default:
    return uml_condition.second;
  }
}

void ActivityData::set_specification(BrowserOperation * op) {
  if (op != specification) {
    if (specification != 0)
      disconnect(specification->get_data(), SIGNAL(deleted()),
		 this, SLOT(on_delete()));
    if ((specification = op) != 0) {
      connect(specification->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete()));
    }
  }
}

void ActivityData::on_delete() {
  if ((specification != 0) && specification->deletedp()) {
    disconnect(specification->get_data(), SIGNAL(deleted()),
	       this, SLOT(on_delete()));
    specification = 0;
  }
}

void ActivityData::edit() {
  setName(browser_node->get_name());
    
  (new ActivityDialog(this))->show();
}

//
  
void ActivityData::send_uml_def(ToolCom * com, BrowserNode * bn,
				const QString & comment) {
  int api = com->api_format();
  
  SimpleData::send_uml_def(com, bn, comment);
  uml_condition.send_def(com);
  com->write_bool(read_only);
  com->write_bool(single_execution);
  if (api >= 45) {
    if (specification == 0)
      com->write_id(0);
    else
      specification->write_id(com);
  
    if (api >= 48) {
      com->write_bool(is_active);
      
      if (api >= 53)
	com->write_string(constraint);
    }
  }
}

void ActivityData::send_cpp_def(ToolCom * com) {
  cpp_condition.send_def(com);
}

void ActivityData::send_java_def(ToolCom * com) {
  java_condition.send_def(com);
}

bool ActivityData::tool_cmd(ToolCom * com, const char * args,
			 BrowserNode * bn,
			 const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setUmlEntryBehaviorCmd:
	uml_condition.first = args;
	break;
      case setUmlExitBehaviorCmd:
	uml_condition.second = args;
	break;
      case setCppEntryBehaviorCmd:
	cpp_condition.first = args;
	break;
      case setCppExitBehaviorCmd:
	cpp_condition.second = args;
	break;
      case setJavaEntryBehaviorCmd:
	java_condition.first = args;
	break;
      case setJavaExitBehaviorCmd:
	java_condition.second = args;
	break;
      case setReadOnlyCmd:
	read_only = (*args != 0);
	break;
      case setSingleExecutionCmd:
	single_execution = (*args != 0);
	break;
      case setDefCmd:
	set_specification((BrowserOperation *) com->get_id(args));
        break;
      case setActiveCmd:
	is_active = (*args != 0);
	break;
      case setConstraintCmd:
	constraint = args;
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

void ActivityData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  uml_condition.save(st, "pre_uml", "post_uml");
  cpp_condition.save(st, "pre_cpp", "post_cpp");
  java_condition.save(st, "pre_java", "post_java");
    
  if (!constraint.isEmpty()) {
    st << "constraint ";
    save_string(constraint, st);
    nl_indent(st);
  }

  bool nl = FALSE;

  if (read_only) {
    nl_indent(st);
    nl = TRUE;
    st << "read_only";
  }

  if (single_execution) {
    if (nl)
      st << " ";
    else {
      nl_indent(st);
      nl = TRUE;
    }
    st << "single_execution";
  }

  if (is_active) {
    if (nl)
      st << " ";
    else {
      nl_indent(st);
      nl = TRUE;
    }
    st << "active";
  }
  
  if (specification != 0) {
    if (nl)
      st << " ";
    else
      nl_indent(st);
    st << "specification ";
    specification->save(st, TRUE, warning);
  }
}

void ActivityData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  uml_condition.read(st, k, "pre_uml", "post_uml");	// updates k
  cpp_condition.read(st, k, "pre_cpp", "post_cpp");	// updates k
  java_condition.read(st, k, "pre_java", "post_java");	// updates k
  
  if (!strcmp(k, "constraint")) {
    constraint = read_string(st);
    k = read_keyword(st);
  }
  else
    constraint = "";
  
  if (! strcmp(k, "read_only")) {
    read_only = TRUE;
    k = read_keyword(st);
  }
  if (! strcmp(k, "single_execution")) {
    single_execution = TRUE;
    k = read_keyword(st);
  }
  if (!strcmp(k, "active")) {
    is_active = TRUE;
    k = read_keyword(st);
  }
  if (! strcmp(k, "specification")) {
    set_specification(BrowserOperation::read_ref(st));
    k = read_keyword(st);
  }
}
