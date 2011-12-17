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

#include "BrowserState.h"
#include "StateData.h"
#include "StateDialog.h"
#include "BrowserOperation.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

StateData::StateData() : is_active(FALSE), specification(0), reference(0) {
}

StateData::StateData(StateData * model, BrowserNode * bn)
    : SimpleData(model), specification(0) {
  browser_node = bn;
  reference = model->reference;
  is_active = model->is_active;
  uml = model->uml;
  cpp = model->cpp;
  java = model->java;
  set_specification(model->specification);
}

StateData::~StateData() {
}

const StateBehavior & StateData::get_behavior(DrawingLanguage ln) const {
  switch (ln) {
  case CppView:
    return cpp;
  case JavaView:
    return java;
  default:
    return uml;
  }
}

void StateData::set_specification(BrowserOperation * op) {
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

void StateData::set_reference(BrowserState * st) {
  if (reference != 0)
    disconnect(reference->get_data(), SIGNAL(deleted()),
	       this, SLOT(on_delete()));
  if ((reference = st) != 0) {
    connect(reference->get_data(), SIGNAL(deleted()),
	    this, SLOT(on_delete()));
  }
}

void StateData::on_delete() {
  if ((specification != 0) && specification->deletedp()) {
    disconnect(specification->get_data(), SIGNAL(deleted()),
	       this, SLOT(on_delete()));
    specification = 0;
  }
  if ((reference != 0) && reference->deletedp()) {
    disconnect(reference->get_data(), SIGNAL(deleted()),
	       this, SLOT(on_delete()));
    reference = 0;
  }
}

void StateData::edit() {
  setName(browser_node->get_name());
    
  (new StateDialog(this))->show();
}

//
  
void StateData::send_uml_def(ToolCom * com, BrowserNode * bn,
			     const QString & comment) {
  int api = com->api_format();
  
  SimpleData::send_uml_def(com, bn, comment);
  uml.send_def(com);
  if (api >= 45) {
    if (specification == 0)
      com->write_id(0);
    else
      specification->write_id(com);
  
    if (api >= 48) {
      com->write_bool(is_active);
      
      if (api >= 55) {
	if (reference == 0)
	  com->write_id(0);
	else
	  reference->write_id(com);
      }
    }
  }
}

void StateData::send_cpp_def(ToolCom * com) {
  cpp.send_def(com);
}

void StateData::send_java_def(ToolCom * com) {
  java.send_def(com);
}

bool StateData::tool_cmd(ToolCom * com, const char * args,
			 BrowserNode * bn,
			 const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    bool ok = TRUE;
    
    if (!bn->is_writable() && !root_permission())
      ok = FALSE;
    else {
      switch ((unsigned char) args[-1]) {
      case setUmlEntryBehaviorCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  uml.on_entry = args;
	break;
      case setUmlExitBehaviorCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  uml.on_exit = args;
	break;
      case setUmlActivityCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  uml.do_activity = args;
	break;
      case setCppEntryBehaviorCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  cpp.on_entry = args;
	break;
      case setCppExitBehaviorCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  cpp.on_exit = args;
	break;
      case setCppActivityCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  cpp.do_activity = args;
	break;
      case setJavaEntryBehaviorCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  java.on_entry = args;
	break;
      case setJavaExitBehaviorCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  java.on_exit = args;
	break;
      case setJavaActivityCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  java.do_activity = args;
	break;
      case setDefCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  set_specification((BrowserOperation *) com->get_id(args));
        break;
      case setActiveCmd:
	if (reference != 0)
	  ok = FALSE;
	else
	  is_active = (*args != 0);
	break;
      case setDerivedCmd:
	{
	  BrowserState * st = (BrowserState *) com->get_id(args);

	  if (st == reference) {
	    com->write_ack(TRUE);
	    return TRUE;
	  }
	  
	  if ((st != 0) &&
	      (!((BrowserState *) bn)->can_reference() ||
	       !((BrowserState *) browser_node)->can_reference(st)))
	    ok = FALSE;
	  else
	    set_reference(st);
	}
	break;
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
    }
    
    if (ok) {
      bn->package_modified();
      modified();
    }
    
    com->write_ack(ok);  
    return TRUE;
  }
  else
    return BasicData::tool_cmd(com, args, bn, comment);
}

//

void StateData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  uml.save(st, "uml");
  cpp.save(st, "cpp");
  java.save(st, "java");

  bool nl = FALSE;

  if (is_active) {
    nl_indent(st);
    nl = TRUE;
    st << "active";
  }
  
  if (specification != 0) {
    if (nl)
      st << " ";
    else {
      nl_indent(st);
      nl = TRUE;
    }
    st << "specification ";
    specification->save(st, TRUE, warning);
  }
  
  if (reference != 0) {
    if (nl)
      st << " ";
    else
      nl_indent(st);
    st << "reference ";
    reference->save(st, TRUE, warning);
  }
}

void StateData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  uml.read(st, k, "uml");	// updates k
  cpp.read(st, k, "cpp");	// updates k
  java.read(st, k, "java");	// updates k
  
  if (!strcmp(k, "active")) {
    is_active = TRUE;
    k = read_keyword(st);
  }
  
  if (! strcmp(k, "specification")) {
    set_specification(BrowserOperation::read_ref(st));
    k = read_keyword(st);
  }
  
  if (! strcmp(k, "reference")) {
    set_reference(BrowserState::read_ref(st));
    k = read_keyword(st);
  }
}

//

void StateBehavior::send_def(ToolCom * com) {
  com->write_string(on_entry);
  com->write_string(on_exit);
  com->write_string(do_activity);
}

void StateBehavior::save(QTextStream & st, const char * lang) const {
  indent(+1);
  
  if (!on_entry.isEmpty()) {
    nl_indent(st);
    st << lang << "_on_entry ";
    save_string(on_entry, st);
  }
  if (!on_exit.isEmpty()) {
    nl_indent(st);
    st << lang << "_on_exit ";
    save_string(on_exit, st);
  }
  if (!do_activity.isEmpty()) {
    nl_indent(st);
    st << lang << "_do_activity ";
    save_string(do_activity, st);
  }
  
  indent(-1);
}

void StateBehavior::read(char * & st, char * & k, const char * lang) {
  int ln = strlen(lang);
  
  if (!strncmp(k, lang, ln) && !strcmp(k+ln, "_on_entry")) {
    on_entry = read_string(st);
    k = read_keyword(st);
  }
  if (!strncmp(k, lang, ln) && !strcmp(k+ln, "_on_exit")) {
    on_exit = read_string(st);
    k = read_keyword(st);
  }
  if (!strncmp(k, lang, ln) && !strcmp(k+ln, "_do_activity")) {
    do_activity = read_string(st);
    k = read_keyword(st);
  }
}
