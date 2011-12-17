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

#include "FlowData.h"
#include "BrowserFlow.h"
#include "FlowDialog.h"
#include "BrowserParameter.h"
#include "BrowserPin.h"
#include "BrowserExpansionNode.h"
#include "BrowserActivityAction.h"
#include "BrowserActivityNode.h"
#include "BrowserActivityObject.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

FlowData::FlowData() : end(0) {
}

FlowData::FlowData(const BrowserFlow * model,
			       BrowserFlow * r)
    : SimpleData(model->get_data()) {
  set_start_end(r, ((FlowData *) model->get_data())->end);
}

FlowData::~FlowData() {
}

void FlowData::delete_it() {
  if (!deletedp()) {
    if (!browser_node->deletedp())
      browser_node->delete_it();
    BasicData::delete_it();
  }
}

void FlowData::set_start_end(BrowserFlow * s, BrowserNode * e) {
  browser_node = s;
  end = e;
  connect(e->get_data(), SIGNAL(deleted()), this, SLOT(end_deleted()));
}

BrowserNode * FlowData::get_start_node() const {
  return (BrowserNode *) browser_node->parent();
}

void FlowData::end_deleted() {
  browser_node->package_modified();
  delete_it();
}

void FlowData::edit() {
  (new FlowDialog(this))->show();
}

// return guard & weight
QString FlowData::str(bool horiz, DrawingLanguage lg) const {
  switch(lg) {
  case CppView:
    return cpp.str(horiz);
  case JavaView:
    return java.str(horiz);
  default:
    return uml.str(horiz);
  }
}

QString FlowData::selection(DrawingLanguage lg) const {
  switch(lg) {
  case CppView:
    return cpp.sel_trans.first;
  case JavaView:
    return java.sel_trans.first;
  default:
    return uml.sel_trans.first;
  }
}

QString FlowData::transformation(DrawingLanguage lg) const {
  switch(lg) {
  case CppView:
    return cpp.sel_trans.second;
  case JavaView:
    return java.sel_trans.second;
  default:
    return uml.sel_trans.second;
  }
}

//

void FlowData::send_uml_def(ToolCom * com,
			    BrowserNode * bn,
			    const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
  get_end_node()->write_id(com);
  uml.send_def(com);
}

void FlowData::send_cpp_def(ToolCom * com) {
  cpp.send_def(com);
}

void FlowData::send_java_def(ToolCom * com) {
  java.send_def(com);
}

bool FlowData::tool_cmd(ToolCom * com, const char * args,
			      BrowserNode * bn,
			      const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setUmlGuardCmd:
	uml.guard = args;
	break;
      case setCppGuardCmd:
	cpp.guard = args;
	break;
      case setJavaGuardCmd:
	java.guard = args;
	break;
      case setUmlActivityCmd:
	uml.weight = args;
	break;
      case setCppActivityCmd:
	cpp.weight = args;
	break;
      case setJavaActivityCmd:
	java.weight = args;
	break;
      case setUmlEntryBehaviorCmd:
	uml.sel_trans.first = args;
	break;
      case setUmlExitBehaviorCmd:
	uml.sel_trans.second = args;
	break;
      case setCppEntryBehaviorCmd:
	cpp.sel_trans.first = args;
	break;
      case setCppExitBehaviorCmd:
	cpp.sel_trans.second = args;
	break;
      case setJavaEntryBehaviorCmd:
	java.sel_trans.first = args;
	break;
      case setJavaExitBehaviorCmd:
	java.sel_trans.second = args;
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

void FlowData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  nl_indent(st);
  st << "on ";
  end->save(st, TRUE, warning);
  uml.save(st, "uml");
  cpp.save(st, "cpp");
  java.save(st, "java");
}

FlowData * FlowData::read(char * & st, char * & k)
{
  FlowData * result = new FlowData;
  
  result->BasicData::read(st, k);	// updates k

  if (strcmp(k, "on"))
    wrong_keyword(k, "on");
  
  k = read_keyword(st);
  
  if (((result->end = BrowserParameter::read(st, k, 0)) == 0) &&
      ((result->end = BrowserPin::read(st, k, 0)) == 0) &&
      ((result->end = BrowserExpansionNode::read(st, k, 0)) == 0) &&
      ((result->end = BrowserActivityAction::read(st, k, 0)) == 0) &&
      ((result->end = BrowserActivityNode::read(st, k, 0)) == 0) &&
      ((result->end = BrowserActivityObject::read(st, k, 0)) == 0))
    wrong_keyword(k, "a ref to a parameter, pin, action, node or object");
  
  connect(result->end->get_data(), SIGNAL(deleted()),
	  result, SLOT(end_deleted()));
  
  k = read_keyword(st);
  result->uml.read(st, k, "uml");		// update k
  result->cpp.read(st, k, "cpp");		// update k
  result->java.read(st, k, "java");		// update k
 
  return result;
}

//

// return guard & weight
QString FlowDef::str(bool horiz) const {
  return (guard.isEmpty())
    ? ((weight.isEmpty())
       ? QString::null
       : weight.SharedStr::operator QString())
    : ((weight.isEmpty())
       ? "[" + guard.MyStr::operator QString() + "]"
       : "[" + guard.MyStr::operator QString() + ((horiz) ? "] " : "]\n") + "{weight=" + ((const char *) weight) + '}');
}

void FlowDef::send_def(ToolCom * com) {
  com->write_string(weight);
  com->write_string(guard);
  sel_trans.send_def(com);
}

void FlowDef::save(QTextStream & st, QCString lang) const {
  indent(+1);
  
  if (!weight.isEmpty()) {
    nl_indent(st);
    st << lang << "_weight ";
    save_string(weight, st);
  }
  if (!guard.isEmpty()) {
    nl_indent(st);
    st << lang << "_guard ";
    save_string(guard, st);
  }
  sel_trans.save(st, lang + "_selection", lang + "_transformation");
  
  indent(-1);
}

void FlowDef::read(char * & st, char * & k, QCString lang)
{
  int ln = strlen(lang);
  
  if (!strncmp(k, lang, ln) && !strcmp(k+ln, "_weight")) {
    weight = read_string(st);
    k = read_keyword(st);
  }
  if (!strncmp(k, lang, ln) && !strcmp(k+ln, "_guard")) {
    guard = read_string(st);
    k = read_keyword(st);
  }
  sel_trans.read(st, k, lang + "_selection", lang + "_transformation");
}
