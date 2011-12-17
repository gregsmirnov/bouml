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





#include <qtextstream.h>
#include <qcursor.h>

#include "TransitionData.h"
#include "BrowserTransition.h"
#include "TransitionDialog.h"
#include "BrowserState.h"
#include "BrowserStateAction.h"
#include "BrowserPseudoState.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

TransitionData::TransitionData() : end(0) {
}

TransitionData::TransitionData(const BrowserTransition * model,
			       BrowserTransition * r)
    : SimpleData(model->get_data()) {
  TransitionData * md = (TransitionData *) model->get_data();
  
  set_start_end(r, md->end);	// set is_internal
  is_internal &= md->is_internal;
}

TransitionData::~TransitionData() {
}

void TransitionData::delete_it() {
  if (!deletedp()) {
    if (!browser_node->deletedp())
      browser_node->delete_it();
    BasicData::delete_it();
  }
}

void TransitionData::set_start_end(BrowserTransition * s, BrowserNode * e) {
  browser_node = s;
  end = e;
  connect(e->get_data(), SIGNAL(deleted()), this, SLOT(end_deleted()));
  
  is_internal = (s->parent() == e);
}

BrowserNode * TransitionData::get_start_node() const {
  return (BrowserNode *) browser_node->parent();
}

void TransitionData::end_deleted() {
  browser_node->package_modified();
  delete_it();
}

void TransitionData::edit() {
  (new TransitionDialog(this))->show();
}

QString TransitionData::str(bool horiz, DrawingLanguage lg) const {
  switch(lg) {
  case CppView:
    return cpp.str(horiz);
  case JavaView:
    return java.str(horiz);
  default:
    return uml.str(horiz);
  }
}

//

void TransitionData::send_uml_def(ToolCom * com,
				  BrowserNode * bn,
				  const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
  get_end_node()->write_id(com);
  uml.send_def(com);
  if (com->api_format() >= 33)
    com->write_bool(is_internal);
}

void TransitionData::send_cpp_def(ToolCom * com) {
  cpp.send_def(com);
}

void TransitionData::send_java_def(ToolCom * com) {
  java.send_def(com);
}

bool TransitionData::tool_cmd(ToolCom * com, const char * args,
			      BrowserNode * bn,
			      const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setUmlTriggerCmd:
	uml.trigger = args;
	break;
      case setUmlGuardCmd:
	uml.guard = args;
	break;
      case setUmlActivityCmd:
	uml.expr = args;
	break;
      case setCppTriggerCmd:
	cpp.trigger = args;
	break;
      case setCppGuardCmd:
	cpp.guard = args;
	break;
      case setCppActivityCmd:
	cpp.expr = args;
	break;
      case setJavaTriggerCmd:
	java.trigger = args;
	break;
      case setJavaGuardCmd:
	java.guard = args;
	break;
      case setJavaActivityCmd:
	java.expr = args;
	break;
      case setIsCppExternalCmd:
	if (*args != 0)
	  is_internal = FALSE;
	else if (get_start_node() != end) {
	  com->write_ack(FALSE);
	  return TRUE;
	}
	else
	  is_internal = TRUE;
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

void TransitionData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  nl_indent(st);
  st << "on ";
  end->save(st, TRUE, warning);
  uml.save(st, "uml");
  cpp.save(st, "cpp");
  java.save(st, "java");
}

TransitionData * TransitionData::read(char * & st, char * & k)
{
  TransitionData * result = new TransitionData;
  
  result->BasicData::read(st, k);	// updates k

  if (strcmp(k, "on"))
    wrong_keyword(k, "on");
  
  k = read_keyword(st);
  
  if (((result->end = BrowserState::read(st, k, 0)) == 0) &&
      ((result->end = BrowserPseudoState::read(st, k, 0)) == 0) &&
      ((result->end = BrowserStateAction::read(st, k, 0)) == 0))
    wrong_keyword(k, "a ref to a state or action or pseudo state");
  
  connect(result->end->get_data(), SIGNAL(deleted()),
	  result, SLOT(end_deleted()));
  
  k = read_keyword(st);
  result->uml.read(st, k, "uml");		// update k
  result->cpp.read(st, k, "cpp");		// update k
  result->java.read(st, k, "java");		// update k
  
  return result;
}

//

QString TransDef::str(bool horiz) const {
  QString r = trigger;
  
  if (! guard.isEmpty())
    r += QString((r.isEmpty()) ? "[" : ((horiz) ? " [" : "\n[")) +
      ((const char *) guard) + "]";
  
  if (! expr.isEmpty())
    r += QString((r.isEmpty()) ? "/ " : ((horiz) ? " / " : "\n/ ")) +
      ((const char *) expr);
  
  return r;
}

void TransDef::send_def(ToolCom * com) {
  com->write_string(trigger);
  com->write_string(guard);
  com->write_string(expr);
}

void TransDef::save(QTextStream & st, const char * lang) const {
  indent(+1);
  
  if (!trigger.isEmpty()) {
    nl_indent(st);
    st << lang << "_trigger ";
    save_string(trigger, st);
  }
  if (!guard.isEmpty()) {
    nl_indent(st);
    st << lang << "_guard ";
    save_string(guard, st);
  }
  if (!expr.isEmpty()) {
    nl_indent(st);
    st << lang << "_expr ";
    save_string(expr, st);
  }
  
  indent(-1);
}

void TransDef::read(char * & st, char * & k, const char * lang)
{
  int ln = strlen(lang);
  
  if (!strncmp(k, lang, ln) && !strcmp(k+ln, "_trigger")) {
    trigger = read_string(st);
    k = read_keyword(st);
  }
  if (!strncmp(k, lang, ln) && !strcmp(k+ln, "_guard")) {
    guard = read_string(st);
    k = read_keyword(st);
  }
  if (!strncmp(k, lang, ln) && !strcmp(k+ln, "_expr")) {
    expr = read_string(st);
    k = read_keyword(st);
  }
}
