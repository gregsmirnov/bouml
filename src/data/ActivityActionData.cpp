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

#include "BrowserActivityAction.h"
#include "ActivityActionData.h"
#include "ActivityActionDialog.h"
#include "BrowserAttribute.h"
#include "BrowserRelation.h"
#include "BrowserActivity.h"
#include "BrowserState.h"
#include "BrowserOperation.h"
#include "BrowserParameter.h"
#include "BrowserPin.h"
#include "AttributeData.h"
#include "RelationData.h"
#include "ParameterData.h"
#include "OperationData.h"
#include "PinData.h"
#include "myio.h"
#include "strutil.h"
#include "ToolCom.h"
#include "mu.h"

void PinDescr::do_connect(BrowserClass *) {
  // do nothing
}

void PinDescr::do_disconnect(BrowserClass *) {
  // do nothing
}

ActivityActionData::ActivityActionData()
    : action(new OpaqueAction) {
}

ActivityActionData::ActivityActionData(ActivityActionData * model, BrowserNode * bn)
    : SimpleData(model) {
  browser_node = bn;
  action = model->action->duplicate();
  uml_condition = model->uml_condition;
  cpp_condition = model->cpp_condition;
  java_condition = model->java_condition;
  constraint = model->constraint;
}

ActivityActionData::ActivityActionData(UmlActionKind k) {
  action = 0;
  set_action_kind(k);
}

ActivityActionData::~ActivityActionData() {
  if (action != 0) {
    // don't disconnect action->depend_on() : may be deleted
    delete action;
  }
}

void ActivityActionData::undepend() {
  BasicData * dp = action->depend_on();

  if (dp != 0)
    disconnect(dp, SIGNAL(deleted()), this, SLOT(depend_deleted()));
}

void ActivityActionData::update_depend() {
  BasicData * dp = action->depend_on();

  if (dp != 0)
    connect(dp, SIGNAL(deleted()), this, SLOT(depend_deleted()));
}

QString ActivityActionData::get_precond(DrawingLanguage ln) const {
  switch (ln) {
  case CppView:
    return cpp_condition.first;
  case JavaView:
    return java_condition.first;
  default:
    return uml_condition.first;
  }
}

QString ActivityActionData::get_postcond(DrawingLanguage ln) const {
  switch (ln) {
  case CppView:
    return cpp_condition.second;
  case JavaView:
    return java_condition.second;
  default:
    return uml_condition.second;
  }
}

QString ActivityActionData::str(DrawingLanguage lang) const {
  return action->str(lang, browser_node->get_name());
}

void ActivityActionData::edit() {
  setName(browser_node->get_name());
    
  (new ActivityActionDialog(this))->show();
}

void ActivityActionData::send_uml_def(ToolCom * com, BrowserNode * bn,
				      const QString & comment) {
  SimpleData::send_uml_def(com, bn, comment);
  uml_condition.send_def(com);
  if (com->api_format() >= 53)
    com->write_string(constraint);
  action->send_def(com, UmlView);
}

void ActivityActionData::send_cpp_def(ToolCom * com) {
  cpp_condition.send_def(com);
  action->send_def(com, CppView);
}

void ActivityActionData::send_java_def(ToolCom * com) {
  java_condition.send_def(com);
  action->send_def(com, JavaView);
}

bool ActivityActionData::tool_cmd(ToolCom * com, const char * args,
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
      case setConstraintCmd:
	constraint = args;
	break;
      default:
	if (! action->tool_cmd(com, args))
	  return BasicData::tool_cmd(com, args, bn, comment);
      }
      
      // ok case
      bn->package_modified();
      modified();
      com->write_ack(TRUE);
    }
  }
  else if (!action->tool_cmd(com, args))
    return BasicData::tool_cmd(com, args, bn, comment);
  
  return TRUE;
}

void ActivityActionData::set_action_kind(UmlActionKind k) {

  if (action != 0) {
    // do not disconnect to depend_on : deletions may be already made
    delete action;
  }
  
  switch (k) {
  case UmlOpaqueAction:
    action = new OpaqueAction;
    break;
  case UmlAcceptEventAction:
    action = new AcceptEventAction;
    break;
  case UmlReadVariableValueAction:
    action = new ReadVariableValueAction;
    break;
  case UmlClearVariableValueAction:
    action = new ClearVariableValueAction;
    break;
  case UmlWriteVariableValueAction:
    action = new WriteVariableValueAction;
    break;
  case UmlAddVariableValueAction:
    action = new AddVariableValueAction;
    break;
  case UmlRemoveVariableValueAction:
    action = new RemoveVariableValueAction;
    break;
  case UmlCallBehaviorAction:
    action = new CallBehaviorAction;
    break;
  case UmlCallOperationAction:
    action = new CallOperationAction;
    break;
  case UmlSendObjectAction:
    action = new SendObjectAction;
    break;
  case UmlSendSignalAction:
    action = new SendSignalAction;
    break;
  case UmlBroadcastSignalAction:
    action = new BroadcastSignalAction;
    break;
  case UmlUnmarshallAction:
    action = new UnmarshallAction;
    break;
  case UmlValueSpecificationAction:
    action = new ValueSpecificationAction;
    break;
  case UmlAcceptCallAction:
    action = new AcceptCallAction;
    break;
  case UmlReplyAction:
    action = new ReplyAction;
    break;
  case UmlCreateObjectAction:
    action = new CreateObjectAction;
    break;
  case UmlDestroyObjectAction:
    action = new DestroyObjectAction;
    break;
  case UmlTestIdentityAction:
    action = new TestIdentityAction;
    break;
  case UmlRaiseExceptionAction:
    action = new RaiseExceptionAction;
    break;
  case UmlReduceAction:
    action = new ReduceAction;
    break;
  default:
    // error
    action = 0;
  }
}

void ActivityActionData::depend_deleted() {
  action->on_delete();
}

void ActivityActionData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  uml_condition.save(st, "pre_uml", "post_uml");
  cpp_condition.save(st, "pre_cpp", "post_cpp");
  java_condition.save(st, "pre_java", "post_java");
  nl_indent(st);
  if (!constraint.isEmpty()) {
    st << "constraint ";
    save_string(constraint, st);
    nl_indent(st);
  }
  st << stringify(action->kind());
  action->save(st, warning);
}

void ActivityActionData::read(char * & st, char * & k) {
  BasicData::read(st, k);		// updates k
  uml_condition.read(st, k, "pre_uml", "post_uml");	// updates k
  cpp_condition.read(st, k, "pre_cpp", "post_cpp");	// updates k
  java_condition.read(st, k, "pre_java", "post_java");	// updates k
  
  if (!strcmp(k, "constraint")) {
    constraint = read_string(st);
    k = read_keyword(st);
  }
  else
    constraint = "";
  
  UmlActionKind kind = activity_action_kind(k);
  
  k = read_keyword(st);
  if (action != 0)
    delete action;

  switch (kind) {
  case UmlOpaqueAction:
    (action = new OpaqueAction())->read(st, k);	// update k
    break;
  case UmlAcceptEventAction:
    (action = new AcceptEventAction())->read(st, k);	// update k
    break;
  case UmlReadVariableValueAction:
    (action = new ReadVariableValueAction())->read(st, k);	// update k
    break;
  case UmlClearVariableValueAction:
    (action = new ClearVariableValueAction())->read(st, k);	// update k
    break;
  case UmlWriteVariableValueAction:
    (action = new WriteVariableValueAction())->read(st, k);	// update k
    break;
  case UmlAddVariableValueAction:
    (action = new AddVariableValueAction())->read(st, k);	// update k
    break;
  case UmlRemoveVariableValueAction:
    (action = new RemoveVariableValueAction())->read(st, k);	// update k
    break;
  case UmlCallBehaviorAction:
    (action = new CallBehaviorAction())->read(st, k);	// update k
    break;
  case UmlCallOperationAction:
    (action = new CallOperationAction())->read(st, k);	// update k
    break;
  case UmlSendObjectAction:
    (action = new SendObjectAction())->read(st, k);	// update k
    break;
  case UmlSendSignalAction:
    (action = new SendSignalAction())->read(st, k);	// update k
    break;
  case UmlBroadcastSignalAction:
    (action = new BroadcastSignalAction())->read(st, k);	// update k
    break;
  case UmlUnmarshallAction:
    (action = new UnmarshallAction())->read(st, k);	// update k
    break;
  case UmlValueSpecificationAction:
    (action = new ValueSpecificationAction())->read(st, k);	// update k
    break;
  case UmlAcceptCallAction:
    (action = new AcceptCallAction())->read(st, k);	// update k
    break;
  case UmlReplyAction:
    (action = new ReplyAction())->read(st, k);	// update k
    break;
  case UmlCreateObjectAction:
    (action = new CreateObjectAction())->read(st, k);	// update k
    break;
  case UmlDestroyObjectAction:
    (action = new DestroyObjectAction())->read(st, k);	// update k
    break;
  case UmlTestIdentityAction:
    (action = new TestIdentityAction())->read(st, k);	// update k
    break;
  case UmlRaiseExceptionAction:
    (action = new RaiseExceptionAction())->read(st, k);	// update k
    break;
  case UmlReduceAction:
    (action = new ReduceAction())->read(st, k);	// update k
    break;
  default:
    // error
    action = 0;
  }

  update_depend();
}

// AnyAction

AnyAction::AnyAction() {
}

AnyAction::~AnyAction() {
}

QValueList<PinDescr> AnyAction::pins() const {
  // no pins by default
  QValueList<PinDescr> r;
  
  return r;
}

bool AnyAction::may_add_pin() const {
  return TRUE;
}

QString AnyAction::str(DrawingLanguage, QString name) const {
  if (name.isEmpty()) {
    name = stringify(kind());
    
    int index = 0;
    
    while ((index = name.find('_', index)) != -1)
      name.replace(index, 1, " ");
  }

  return name;
}

BasicData * AnyAction::depend_on() {
  // no dependency
  return 0;
}

void AnyAction::on_delete() {
  // never called
}

BrowserNode * AnyAction::referenced(const char *&) const {
  return 0;
}

bool AnyAction::tool_cmd(ToolCom *, const char *) {
  // cmd not managed at this level
  return FALSE;
}

// OpaqueAction

OpaqueAction::OpaqueAction() {
}

OpaqueAction::~OpaqueAction() {
}

AnyAction * OpaqueAction::duplicate() const {
  OpaqueAction * r = new OpaqueAction;
  
  r->uml_behavior = uml_behavior;
  r->cpp_behavior = cpp_behavior;
  r->java_behavior = java_behavior;

  return r;
}

UmlActionKind OpaqueAction::kind() const {
  return UmlOpaqueAction;
}

QString OpaqueAction::str(DrawingLanguage lang, QString name) const {
  QString s;
  
  switch (lang) {
  case UmlView:
    s = uml_behavior.MyStr::operator QString();
    break;
  case CppView:
    s = cpp_behavior.MyStr::operator QString();
    break;
  case JavaView:
    s = java_behavior.MyStr::operator QString();
    break;
  default:
    // means : don t write definition
    break;
  }
  
  return (s.isEmpty()) 
    ? AnyAction::str(lang, name)
    : toUnicode(s);
}

void OpaqueAction::save(QTextStream & st, QString &) const {
  if (! uml_behavior.isEmpty()) {
    nl_indent(st);
    st << "uml_behavior ";
    save_string(uml_behavior, st);
  }
  if (! cpp_behavior.isEmpty()) {
    nl_indent(st);
    st << "cpp_behavior ";
    save_string(cpp_behavior, st);
  }
  if (! java_behavior.isEmpty()) {
    nl_indent(st);
    st << "java_behavior ";
    save_string(java_behavior, st);
  }
}

void OpaqueAction::read(char * & st, char * & k) {
  if (! strcmp(k, "uml_behavior")) {
    uml_behavior = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "cpp_behavior")) {
    cpp_behavior = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "java_behavior")) {
    java_behavior = read_string(st);
    k = read_keyword(st);
  }
}

void OpaqueAction::send_def(ToolCom * com, DrawingLanguage lang) {
  switch (lang) {
  case UmlView:
    com->write_string(uml_behavior);
    break;
  case CppView:
    com->write_string(cpp_behavior);
    break;
  case JavaView:
    com->write_string(java_behavior);
    break;
  default:
    break;
  }
}

bool OpaqueAction::tool_cmd(ToolCom *, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setUmlActivityCmd:
    uml_behavior = args;
    break;
  case setCppActivityCmd:
    cpp_behavior = args;
    break;
  case setJavaActivityCmd:
    java_behavior = args;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// AcceptEventAction

AcceptEventAction::AcceptEventAction()
    : unmarshall(FALSE), timeevent(FALSE) {
}

AcceptEventAction::~AcceptEventAction() {
}

AnyAction * AcceptEventAction::duplicate() const {
  AcceptEventAction * r = new AcceptEventAction;
  
  r->unmarshall = unmarshall;
  r->timeevent = timeevent;
  r->uml_trigger = uml_trigger;
  r->cpp_trigger = cpp_trigger;
  r->java_trigger = java_trigger; 
  
  return r;
}

UmlActionKind AcceptEventAction::kind() const {
  return UmlAcceptEventAction;
}

void AcceptEventAction::save(QTextStream & st, QString &) const {
  if (unmarshall) {
    nl_indent(st);
    st << "unmarshall";
    if (timeevent)
      st << " timeevent";
  }
  else if (timeevent) {
    nl_indent(st);
    st << "timeevent";
  }
  
  if (! uml_trigger.isEmpty()) {
    nl_indent(st);
    st << "uml_trigger ";
    save_string(uml_trigger, st);
  }
  
  if (! cpp_trigger.isEmpty()) {
    nl_indent(st);
    st << "cpp_trigger ";
    save_string(cpp_trigger, st);
  }
  
  if (! java_trigger.isEmpty()) {
    nl_indent(st);
    st << "java_trigger ";
    save_string(java_trigger, st);
  }
}

void AcceptEventAction::read(char * & st, char * & k) {
  if (! strcmp(k, "unmarshall")) {
    unmarshall = TRUE;
    k = read_keyword(st);
  }
  if (! strcmp(k, "timeevent")) {
    timeevent = TRUE;
    k = read_keyword(st);
  }
  if (! strcmp(k, "uml_trigger")) {
    uml_trigger = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "cpp_trigger")) {
    cpp_trigger = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "java_trigger")) {
    java_trigger = read_string(st);
    k = read_keyword(st);
  }
}

void AcceptEventAction::send_def(ToolCom * com, DrawingLanguage lang) {
  switch (lang) {
  case UmlView:
    com->write_bool(unmarshall);
    com->write_bool(timeevent);
    com->write_string(uml_trigger);
    break;
  case CppView:
    com->write_string(cpp_trigger);
    break;
  case JavaView:
    com->write_string(java_trigger);
    break;
  default:
    break;
  }
}

bool AcceptEventAction::tool_cmd(ToolCom *, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setUmlTriggerCmd:
    uml_trigger = args;
    break;
  case setCppTriggerCmd:
    cpp_trigger = args;
    break;
  case setJavaTriggerCmd:
    java_trigger = args;
    break;
  case setUnmarshallCmd:
    unmarshall = *args != 0;
    break;
  case setTimeEventCmd:
    timeevent = *args != 0;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// AccessVariableValueAction

AccessVariableValueAction::AccessVariableValueAction()
    : variable(0) {
}

AccessVariableValueAction::~AccessVariableValueAction() {
}

AnyAction * AccessVariableValueAction::duplicate(AccessVariableValueAction * r) const {
  r->variable = variable;
  
  return r;
}

QValueList<PinDescr> AccessVariableValueAction::pins(UmlParamDirection dir,
						     const char * str) const {
  // [in/out] <str> : le type de la variable
  QValueList<PinDescr> r;
  
  if (variable != 0) {
    PinDescr p;

    p.dir = dir;
    p.name = str;
    if (variable->get_type() == UmlAttribute)
      p.set_type(((AttributeData *) variable->get_data())->get_type());
    else {
      RelationData * rd = (RelationData *) variable->get_data();
      bool role_a = rd->is_a((BrowserRelation *) variable);

      if (role_a) {
	p.set_type(rd->get_end_class());
	p.multiplicity = rd->get_multiplicity_a();
      }
      else {
	p.set_type(rd->get_start_class());
	p.multiplicity = rd->get_multiplicity_b();
      }
    }
    r.append(p);
  }

  return r;
}

BasicData * AccessVariableValueAction::depend_on() {
  return (variable) ? variable->get_data() : 0;
}

void AccessVariableValueAction::on_delete() {
  variable = 0;
}

BrowserNode * AccessVariableValueAction::referenced(const char *& s) const {
  if (variable != 0)
    s = "variable";
  return variable;
}

void AccessVariableValueAction::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  
  if (variable == 0)
    st << "no_variable";
  else {
    switch (variable->get_type()) {
    case UmlAttribute:
      ((BrowserAttribute *) variable)->save(st, TRUE, warning);
      break;
    default:
      // a relation
      ((BrowserRelation *) variable)->save(st, TRUE, warning);
      break;
    }
  }
}

void AccessVariableValueAction::read(char * & st, char * & k) {
  if (strcmp(k, "no_variable")) {
    if (((variable = BrowserAttribute::read(st, k, 0)) == 0) &&
	((variable = BrowserRelation::read(st, k, 0)) == 0))
      wrong_keyword(k, "no_variable or attribute_ref or relation_ref");
  }
   
  k = read_keyword(st);
}

void AccessVariableValueAction::send_def(ToolCom * com, DrawingLanguage lang) {
  if (lang == UmlView) {
    if (variable != 0)
      variable->write_id(com);
    else
      com->write_id(0);
  }
}

bool AccessVariableValueAction::tool_cmd(ToolCom * com, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setDefCmd:
    {
      BrowserNode * bn = (BrowserNode *) com->get_id(args);
      UmlCode k = bn->get_type();
      
      if ((k == UmlAttribute) || IsaRelation(k))
	variable = bn;
      else
	return FALSE;
    }
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// ChangeVariableValueAction

ChangeVariableValueAction::ChangeVariableValueAction()
    : flag(FALSE) {
}

ChangeVariableValueAction::~ChangeVariableValueAction() {
}

AnyAction * ChangeVariableValueAction::duplicate(ChangeVariableValueAction * r) const {
  r->AccessVariableValueAction::duplicate(r);
  r->flag = flag;
  
  return r;
}

QValueList<PinDescr> ChangeVariableValueAction::pins(const char * str) const {
  // [in] "value" : le type de la variable
  // [in] <str> : UnlimitedNatural
  QValueList<PinDescr> r = AccessVariableValueAction::pins(UmlIn, "value");
  
  if (! r.isEmpty()) {
    r.first().multiplicity = "";

    PinDescr p;

    p.name = str;
    
    AType t;
    
    p.set_type(t);
    
    r.append(p);
  }

  return r;
}

void ChangeVariableValueAction::save(QTextStream & st, QString & warning,
				     const char * str) const {
  if (flag) {
    nl_indent(st);
    st << str << ' ';
  }
  
  AccessVariableValueAction::save(st, warning);
}

void ChangeVariableValueAction::read(char * & st, char * & k,
				     const char * str) {
  if (!strcmp(k, str)) {
    flag = TRUE;
    k = read_keyword(st);
  }
  
  AccessVariableValueAction::read(st, k);
}

void ChangeVariableValueAction::send_def(ToolCom * com, DrawingLanguage lang) {  
  AccessVariableValueAction::send_def(com, lang);
  
  if (lang == UmlView)
    com->write_bool(flag);
}

bool ChangeVariableValueAction::tool_cmd(ToolCom * com, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setFlagCmd:
    flag = *args != 0;
    break;
  default:
    return AccessVariableValueAction::tool_cmd(com, args);
  }
      
  // ok case
  return TRUE;
}

// read variable value

ReadVariableValueAction::ReadVariableValueAction() {
}

ReadVariableValueAction::~ReadVariableValueAction() {
}

AnyAction * ReadVariableValueAction::duplicate() const {
  return AccessVariableValueAction::duplicate(new ReadVariableValueAction);
}

UmlActionKind ReadVariableValueAction::kind() const {
  return UmlReadVariableValueAction;
}

QValueList<PinDescr> ReadVariableValueAction::pins() const {
  return AccessVariableValueAction::pins(UmlOut, "result");
}

// clear variable value

ClearVariableValueAction::ClearVariableValueAction() {
}

ClearVariableValueAction::~ClearVariableValueAction() {
}

AnyAction * ClearVariableValueAction::duplicate() const {
  return AccessVariableValueAction::duplicate(new ClearVariableValueAction);
}

UmlActionKind ClearVariableValueAction::kind() const {
  return UmlClearVariableValueAction;
}

// write variable value

WriteVariableValueAction::WriteVariableValueAction() {
}

WriteVariableValueAction::~WriteVariableValueAction() {
}

AnyAction * WriteVariableValueAction::duplicate() const {
  return AccessVariableValueAction::duplicate(new WriteVariableValueAction);
}

UmlActionKind WriteVariableValueAction::kind() const {
  return UmlWriteVariableValueAction;
}

QValueList<PinDescr> WriteVariableValueAction::pins() const {
  return AccessVariableValueAction::pins(UmlIn, "value");
}

// AddVariableValueAction

AddVariableValueAction::AddVariableValueAction() {
}

AddVariableValueAction::~AddVariableValueAction() {
}

AnyAction * AddVariableValueAction::duplicate() const {
  return ChangeVariableValueAction::duplicate(new AddVariableValueAction);
}

UmlActionKind AddVariableValueAction::kind() const {
  return UmlAddVariableValueAction;
}

QValueList<PinDescr> AddVariableValueAction::pins() const {
  return ChangeVariableValueAction::pins("insertAt");
}

void AddVariableValueAction::save(QTextStream & st, QString & warning) const {
  ChangeVariableValueAction::save(st, warning, "replace_all");
}

void AddVariableValueAction::read(char * & st, char * & k) {
  ChangeVariableValueAction::read(st, k, "replace_all");
}

// RemoveVariableValueAction

RemoveVariableValueAction::RemoveVariableValueAction() {
}

RemoveVariableValueAction::~RemoveVariableValueAction() {
}

AnyAction * RemoveVariableValueAction::duplicate() const {
  return ChangeVariableValueAction::duplicate(new RemoveVariableValueAction);
}

UmlActionKind RemoveVariableValueAction::kind() const {
  return UmlRemoveVariableValueAction;
}

QValueList<PinDescr> RemoveVariableValueAction::pins() const {
  return ChangeVariableValueAction::pins("removeAt");
}

void RemoveVariableValueAction::save(QTextStream & st, QString & warning) const {
  ChangeVariableValueAction::save(st, warning, "remove_duplicates");
}

void RemoveVariableValueAction::read(char * & st, char * & k) {
  ChangeVariableValueAction::read(st, k, "remove_duplicates");
}

// CallBehaviorAction

CallBehaviorAction::CallBehaviorAction()
: synchronous(FALSE), behavior(0) {
}

CallBehaviorAction::~CallBehaviorAction() {
}

AnyAction * CallBehaviorAction::duplicate() const {
  CallBehaviorAction * r = new CallBehaviorAction;

  r->synchronous = synchronous;
  r->behavior = behavior;

  return r;
}

UmlActionKind CallBehaviorAction::kind() const {
  return UmlCallBehaviorAction;
}

QValueList<PinDescr> CallBehaviorAction::pins() const {
  QValueList<PinDescr> r;

  if ((behavior != 0) && (behavior->get_type() == UmlActivity)) {
    // [any]* : depend on activity's parameter
    QValueList<BrowserParameter *> l = 
      ((BrowserActivity *) behavior)->get_params();
    QValueList<BrowserParameter *>::ConstIterator iter;

    for (iter = l.begin(); iter != l.end(); ++iter) {
      PinParamData * model = (ParameterData *) (*iter)->get_data();
      PinDescr p;
      
      p.name = (*iter)->get_name();
      p.set_type(model->get_type());
      p.multiplicity = model->multiplicity;
      p.in_state = model->in_state;
      p.ordering = model->ordering;
      p.is_control = model->is_control;
      p.unique = model->unique;
      p.exception = model->exception;
      p.stream = model->stream;
      p.dir = model->dir;
      p.effect = model->effect;

      switch (p.dir) {
      case UmlInOut:
	p.dir = UmlIn;
	r.append(p);
	// no break
      case UmlReturn:
	p.dir = UmlOut;
      default:
	break;
      }
      r.append(p);
    }
  }

  return r;
}
  
BasicData * CallBehaviorAction::depend_on() {
  return (behavior) ? behavior->get_data() : 0;
}

void CallBehaviorAction::on_delete() {
  behavior = 0;
}

BrowserNode * CallBehaviorAction::referenced(const char *& s) const {
  if (behavior != 0)
    s = "behavior";
  return behavior;
}

void CallBehaviorAction::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  
  if (synchronous)
    st << "synchronous ";
  
  if (behavior == 0)
    st << "no_behavior";
  else {
    switch (behavior->get_type()) {
    case UmlActivity:
      ((BrowserActivity *) behavior)->save(st, TRUE, warning);
      break;
    default:
      // a state machine
      ((BrowserState *) behavior)->save(st, TRUE, warning);
      break;
    }
  }
}

void CallBehaviorAction::read(char * & st, char * & k) {
  if (!strcmp(k, "synchronous")) {
    synchronous = TRUE;
    k = read_keyword(st);
  }
  
  if (strcmp(k, "no_behavior")) {
    if (((behavior = BrowserActivity::read(st, k, 0)) == 0) &&
	((behavior = BrowserState::read(st, k, 0)) == 0))
      wrong_keyword(k, "no_behavior or activity_ref or state_ref");
  }
   
  k = read_keyword(st);
}

void CallBehaviorAction::send_def(ToolCom * com, DrawingLanguage lang) {
  if (lang == UmlView) {
    com->write_bool(synchronous);
    if (behavior != 0)
      behavior->write_id(com);
    else
      com->write_id(0);
  }
}

bool CallBehaviorAction::tool_cmd(ToolCom * com, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setDefCmd:
    {
      BrowserNode * bn = (BrowserNode *) com->get_id(args);
      
      switch (bn->get_type()) {
      case UmlActivity:
      case UmlState:
	behavior = bn;
	break;
      default:
	return FALSE;
      }
    }
    break;
  case setFlagCmd:
    synchronous = *args != 0;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// CallOperationAction

CallOperationAction::CallOperationAction() 
  : synchronous(FALSE), operation(0) {
}

CallOperationAction::~CallOperationAction() {
}

AnyAction * CallOperationAction::duplicate() const {
  CallOperationAction * r = new CallOperationAction;

  r->synchronous = synchronous;
  r->operation = operation;

  return r;
}

UmlActionKind CallOperationAction::kind() const {
  return UmlCallOperationAction;
}

QValueList<PinDescr> CallOperationAction::pins() const {
  // [in] "target" : instance,
  // [any]* : other depend on operation's params&result&exception
  QValueList<PinDescr> r;

  if (operation != 0) {
    OperationData * d = (OperationData *) operation->get_data();
    PinDescr target;

    target.dir = UmlIn;
    target.name = "target";
    target.set_type((BrowserClass *) operation->parent());
    r.append(target);

    const AType & rt = d->get_return_type();

    if ((rt.type != 0) ||
	(!rt.explicit_type.isEmpty() && (rt.explicit_type != "void"))) {
      PinDescr p;

      p.dir = UmlReturn;
      p.set_type(rt);
      r.append(p);
    }

    int n;
    int i;

    n = (int) d->get_n_params();
    for (i = 0; i != n; i += 1) {
      PinDescr p;

      p.name = d->get_param_name(i);
      p.set_type(d->get_param_type(i));
      p.dir = d->get_param_dir(i);

      if (p.dir == UmlInOut) {
	p.dir = UmlIn;
	r.append(p);
	p.dir = UmlOut;
      }
      r.append(p);
    }

    n = (int) d->get_n_exceptions();
    for (i = 0; i != n; i += 1) {
      PinDescr p;

      p.set_type(d->get_exception(i));
      p.dir = UmlOut;
      p.exception = TRUE;
      r.append(p);
    }
  }

  return r;
}
  
QString CallOperationAction::str(DrawingLanguage lang, QString name) const {
  if (operation == 0)
    return AnyAction::str(lang, name);
  else if (name.isEmpty())
    return operation->get_name();
  else
    return name;
}

BasicData * CallOperationAction::depend_on() {
  return (operation) ? operation->get_data() : 0;
}

BrowserNode * CallOperationAction::referenced(const char *& s) const {
  if (operation != 0)
    s = "operation";
  return operation;
}

void CallOperationAction::on_delete() {
  operation = 0;
}

void CallOperationAction::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  
  if (synchronous)
    st << "synchronous ";
  
  if (operation == 0)
    st << "no_operation";
  else
    operation->save(st, TRUE, warning);
}

void CallOperationAction::read(char * & st, char * & k) {
  if (!strcmp(k, "synchronous")) {
    synchronous = TRUE;
    k = read_keyword(st);
  }
  
  if (strcmp(k, "no_operation")) {
    if ((operation = BrowserOperation::read(st, k, 0)) == 0)
      wrong_keyword(k, "no_operation or operation_ref");
  }
   
  k = read_keyword(st);
}

void CallOperationAction::send_def(ToolCom * com, DrawingLanguage lang) {
  if (lang == UmlView) {
    com->write_bool(synchronous);
    if (operation != 0)
      operation->write_id(com);
    else
      com->write_id(0);
  }
}

bool CallOperationAction::tool_cmd(ToolCom * com, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setDefCmd:
    operation = (BrowserOperation *) com->get_id(args);
    break;
  case setFlagCmd:
    synchronous = *args != 0;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// SendObjectAction

SendObjectAction::SendObjectAction() {
}

SendObjectAction::~SendObjectAction() {
}

AnyAction * SendObjectAction::duplicate() const {
  return new SendObjectAction;

}

UmlActionKind SendObjectAction::kind() const {
  return UmlSendObjectAction;
}

QValueList<PinDescr> SendObjectAction::pins() const {
  // [in] "request" : the sent object
  // [in] "target" : target object,
  // [in]* : arguments (pin addable et removables)
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlIn;
  p.name = "request";
  r.append(p);

  p.name = "target";
  r.append(p);

  return r;
}
  
void SendObjectAction::save(QTextStream &, QString &) const {
  // does nothing
}

void SendObjectAction::read(char * &, char * &) {
  // does nothing
}

void SendObjectAction::send_def(ToolCom *, DrawingLanguage) {
  // does nothing
}

// SendSignalAction

SendSignalAction::SendSignalAction() {
}

SendSignalAction::~SendSignalAction() {
}

AnyAction * SendSignalAction::duplicate() const {
  SendSignalAction * r = new SendSignalAction;

  r->ocl_signal = ocl_signal;
  r->cpp_signal = cpp_signal;
  r->java_signal = java_signal;
  
  return r;
}

UmlActionKind SendSignalAction::kind() const {
  return UmlSendSignalAction;
}

QValueList<PinDescr> SendSignalAction::pins() const {
  // [in] "target"
  // [*] signal params
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlIn;
  p.name = "target";
  r.append(p);

  return r;
}
  
void SendSignalAction::save(QTextStream & st, QString &) const {
  if (! ocl_signal.isEmpty()) {
    nl_indent(st);
    st << "uml ";
    save_string(ocl_signal, st);
  }
  if (! cpp_signal.isEmpty()) {
    nl_indent(st);
    st << "cpp ";
    save_string(cpp_signal, st);
  }
  if (! java_signal.isEmpty()) {
    nl_indent(st);
    st << "java ";
    save_string(java_signal, st);
  }
}

void SendSignalAction::read(char * & st, char * & k) {
  if (! strcmp(k, "uml")) {
    ocl_signal = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "cpp")) {
    cpp_signal = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "java")) {
    java_signal = read_string(st);
    k = read_keyword(st);
  }
}

void SendSignalAction::send_def(ToolCom * com, DrawingLanguage lang) {
  switch (lang) {
  case UmlView:
    com->write_string(ocl_signal);
    break;
  case CppView:
    com->write_string(cpp_signal);
    break;
  case JavaView:
    com->write_string(java_signal);
    break;
  default:
    break;
  }
}

bool SendSignalAction::tool_cmd(ToolCom *, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setUmlActivityCmd:
    ocl_signal = args;
    break;
  case setCppActivityCmd:
    cpp_signal = args;
    break;
  case setJavaActivityCmd:
    java_signal = args;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// BroadcastSignalAction

BroadcastSignalAction::BroadcastSignalAction() {
}

BroadcastSignalAction::~BroadcastSignalAction() {
}

AnyAction * BroadcastSignalAction::duplicate() const {
  BroadcastSignalAction * r = new BroadcastSignalAction;

  r->ocl_signal = ocl_signal;
  r->cpp_signal = cpp_signal;
  r->java_signal = java_signal;
  
  return r;
}

UmlActionKind BroadcastSignalAction::kind() const {
  return UmlBroadcastSignalAction;
}

QValueList<PinDescr> BroadcastSignalAction::pins() const {
  // [*] signal params
  QValueList<PinDescr> r;

  return r;
}
  
// UnmarshallAction

UnmarshallAction::UnmarshallAction() {
}

UnmarshallAction::~UnmarshallAction() {
}

AnyAction * UnmarshallAction::duplicate() const {
  return new UnmarshallAction;
}

UmlActionKind UnmarshallAction::kind() const {
  return UmlUnmarshallAction;
}

QValueList<PinDescr> UnmarshallAction::pins() const {
  // [in] "object" : the unmashalled object
  // [out]* : the objects (pin addable et removables)
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlIn;
  p.name = "object";
  r.append(p);

  return r;
}
  
void UnmarshallAction::save(QTextStream &, QString &) const {
  // does nothing
}

void UnmarshallAction::read(char * &, char * &) {
  // does nothing
}

void UnmarshallAction::send_def(ToolCom *, DrawingLanguage) {
  // does nothing
}

// ValueSpecificationAction

ValueSpecificationAction::ValueSpecificationAction() {
}

ValueSpecificationAction::~ValueSpecificationAction() {
}

AnyAction * ValueSpecificationAction::duplicate() const {
  ValueSpecificationAction * r = new ValueSpecificationAction;

  r->uml_value = uml_value;
  r->cpp_value = cpp_value;
  r->java_value = java_value;

  return r;
}

UmlActionKind ValueSpecificationAction::kind() const {
  return UmlValueSpecificationAction;
}

QValueList<PinDescr> ValueSpecificationAction::pins() const {
  // [out] : the value
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlOut;
  r.append(p);

  return r;
}

bool ValueSpecificationAction::may_add_pin() const {
  return FALSE;
}

void ValueSpecificationAction::save(QTextStream & st, QString &) const {
  if (! uml_value.isEmpty()) {
    nl_indent(st);
    st << "uml_value ";
    save_string(uml_value, st);
  }
  if (! cpp_value.isEmpty()) {
    nl_indent(st);
    st << "cpp_value ";
    save_string(cpp_value, st);
  }
  if (! java_value.isEmpty()) {
    nl_indent(st);
    st << "java_value ";
    save_string(java_value, st);
  }
}

void ValueSpecificationAction::read(char * & st, char * & k) {
  if (!strcmp(k, "uml_value")) {
    uml_value = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "cpp_value")) {
    cpp_value = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, "java_value")) {
    java_value = read_string(st);
    k = read_keyword(st);
  }
}

void ValueSpecificationAction::send_def(ToolCom * com, DrawingLanguage lang) {
  switch (lang) {
  case UmlView:
    com->write_string(uml_value);
    break;
  case CppView:
    com->write_string(cpp_value);
    break;
  case JavaView:
    com->write_string(java_value);
    break;
  default:
    break;
  }
}

bool ValueSpecificationAction::tool_cmd(ToolCom *, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setUmlActivityCmd:
    uml_value = args;
    break;
  case setCppActivityCmd:
    cpp_value = args;
    break;
  case setJavaActivityCmd:
    java_value = args;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// AcceptCallAction

AcceptCallAction::AcceptCallAction() {
}

AcceptCallAction::~AcceptCallAction() {
}

AnyAction * AcceptCallAction::duplicate() const {
  AcceptCallAction * r = new AcceptCallAction;
  
  r->uml_trigger = uml_trigger;
  r->cpp_trigger = cpp_trigger;
  r->java_trigger = java_trigger; 
  
  return r;
}

UmlActionKind AcceptCallAction::kind() const {
  return UmlAcceptCallAction;
}

QValueList<PinDescr> AcceptCallAction::pins() const {
  // [out] : returnInformation
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlOut;
  p.name = "returnInformation";
  r.append(p);

  return r;
}

void AcceptCallAction::save(QTextStream & st, QString &) const {
  if (! uml_trigger.isEmpty()) {
    nl_indent(st);
    st << "uml_trigger ";
    save_string(uml_trigger, st);
  }
  
  if (! cpp_trigger.isEmpty()) {
    nl_indent(st);
    st << "cpp_trigger ";
    save_string(cpp_trigger, st);
  }
  
  if (! java_trigger.isEmpty()) {
    nl_indent(st);
    st << "java_trigger ";
    save_string(java_trigger, st);
  }
}

void AcceptCallAction::read(char * & st, char * & k) {
  if (! strcmp(k, "uml_trigger")) {
    uml_trigger = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "cpp_trigger")) {
    cpp_trigger = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "java_trigger")) {
    java_trigger = read_string(st);
    k = read_keyword(st);
  }
}

void AcceptCallAction::send_def(ToolCom * com, DrawingLanguage lang) {
  switch (lang) {
  case UmlView:
    com->write_string(uml_trigger);
    break;
  case CppView:
    com->write_string(cpp_trigger);
    break;
  case JavaView:
    com->write_string(java_trigger);
    break;
  default:
    break;
  }
}

bool AcceptCallAction::tool_cmd(ToolCom *, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setUmlTriggerCmd:
    uml_trigger = args;
    break;
  case setCppTriggerCmd:
    cpp_trigger = args;
    break;
  case setJavaTriggerCmd:
    java_trigger = args;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// ReplyAction

ReplyAction::ReplyAction() {
}

ReplyAction::~ReplyAction() {
}

AnyAction * ReplyAction::duplicate() const {
  ReplyAction * r = new ReplyAction;
  
  r->uml_trigger = uml_trigger;
  r->cpp_trigger = cpp_trigger;
  r->java_trigger = java_trigger; 
  
  return r;
}

UmlActionKind ReplyAction::kind() const {
  return UmlReplyAction;
}

QValueList<PinDescr> ReplyAction::pins() const {
  // [in] : returnInformation
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlIn;
  p.name = "returnInformation";
  r.append(p);

  return r;
}

void ReplyAction::save(QTextStream & st, QString &) const {
  if (! uml_trigger.isEmpty()) {
    nl_indent(st);
    st << "uml_trigger ";
    save_string(uml_trigger, st);
  }
  
  if (! cpp_trigger.isEmpty()) {
    nl_indent(st);
    st << "cpp_trigger ";
    save_string(cpp_trigger, st);
  }
  
  if (! java_trigger.isEmpty()) {
    nl_indent(st);
    st << "java_trigger ";
    save_string(java_trigger, st);
  }
}

void ReplyAction::read(char * & st, char * & k) {
  if (! strcmp(k, "uml_trigger")) {
    uml_trigger = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "cpp_trigger")) {
    cpp_trigger = read_string(st);
    k = read_keyword(st);
  }
  if (! strcmp(k, "java_trigger")) {
    java_trigger = read_string(st);
    k = read_keyword(st);
  }
}

void ReplyAction::send_def(ToolCom * com, DrawingLanguage lang) {
  switch (lang) {
  case UmlView:
    com->write_string(uml_trigger);
    break;
  case CppView:
    com->write_string(cpp_trigger);
    break;
  case JavaView:
    com->write_string(java_trigger);
    break;
  default:
    break;
  }
}

bool ReplyAction::tool_cmd(ToolCom *, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setUmlTriggerCmd:
    uml_trigger = args;
    break;
  case setCppTriggerCmd:
    cpp_trigger = args;
    break;
  case setJavaTriggerCmd:
    java_trigger = args;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// CreateObjectAction

CreateObjectAction::CreateObjectAction() {
}

CreateObjectAction::~CreateObjectAction() {
}

AnyAction * CreateObjectAction::duplicate() const {
  CreateObjectAction * r = new CreateObjectAction;
  
  r->classifier = classifier;
  
  return r;
}

UmlActionKind CreateObjectAction::kind() const {
  return UmlCreateObjectAction;
}

QValueList<PinDescr> CreateObjectAction::pins() const {
  // [out] : return
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlOut;
  p.name = "return";
  r.append(p);

  return r;
}

void CreateObjectAction::save(QTextStream & st, QString &) const {
  if (! classifier.isEmpty()) {
    nl_indent(st);
    st << "classifier ";
    save_string(classifier, st);
  }
}

void CreateObjectAction::read(char * & st, char * & k) {
  if (! strcmp(k, "classifier")) {
    classifier = read_string(st);
    k = read_keyword(st);
  }
}

void CreateObjectAction::send_def(ToolCom * com, DrawingLanguage lang) {
  if (lang == UmlView) {
    com->write_string(classifier);
  }
}

bool CreateObjectAction::tool_cmd(ToolCom *, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setDefCmd:
    classifier = args;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// DestroyObjectAction

DestroyObjectAction::DestroyObjectAction()
    : is_destroy_links(FALSE), is_destroy_owned_objects(FALSE) {
}

DestroyObjectAction::~DestroyObjectAction() {
}

AnyAction * DestroyObjectAction::duplicate() const {
  DestroyObjectAction * r = new DestroyObjectAction;
  
  r->is_destroy_links = is_destroy_links;
  r->is_destroy_owned_objects = is_destroy_owned_objects;
  
  return r;
}

UmlActionKind DestroyObjectAction::kind() const {
  return UmlDestroyObjectAction;
}

QValueList<PinDescr> DestroyObjectAction::pins() const {
  // [in] : target
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlIn;
  p.name = "target";
  r.append(p);

  return r;
}

bool DestroyObjectAction::may_add_pin() const {
  return FALSE;
}

void DestroyObjectAction::save(QTextStream & st, QString &) const {  
  if (is_destroy_links) {
    nl_indent(st);
    st << "is_destroy_links ";
    if (is_destroy_owned_objects)
      st << "is_destroy_owned_objects";
  }
  else if (is_destroy_owned_objects) {
    nl_indent(st);
    st << "is_destroy_owned_objects";
  }
}

void DestroyObjectAction::read(char * & st, char * & k) {
  if (! strcmp(k, "is_destroy_links")) {
    is_destroy_links = TRUE;
    k = read_keyword(st);
  }
  if (! strcmp(k, "is_destroy_owned_objects")) {
    is_destroy_owned_objects = TRUE;
    k = read_keyword(st);
  }
}

void DestroyObjectAction::send_def(ToolCom * com, DrawingLanguage lang) {
  if (lang == UmlView) {
    com->write_bool(is_destroy_links);
    com->write_bool(is_destroy_owned_objects);
  }
}

bool DestroyObjectAction::tool_cmd(ToolCom *, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setTypeCmd:
    is_destroy_links = (*args != 0);
    break;
  case setFlagCmd:
    is_destroy_owned_objects = (*args != 0);
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}

// TestIdentityAction

TestIdentityAction::TestIdentityAction() {
}

TestIdentityAction::~TestIdentityAction() {
}

AnyAction * TestIdentityAction::duplicate() const {
  return new TestIdentityAction;
}

UmlActionKind TestIdentityAction::kind() const {
  return UmlTestIdentityAction;
}

QValueList<PinDescr> TestIdentityAction::pins() const {
  // [in] : first
  // [in] : second
  // [out] : result
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlIn;
  p.name = "first";
  r.append(p);

  p.dir = UmlIn;
  p.name = "second";
  r.append(p);

  p.dir = UmlOut;
  p.name = "result";
  r.append(p);

  return r;
}
  
bool TestIdentityAction::may_add_pin() const {
  return FALSE;
}

void TestIdentityAction::save(QTextStream &, QString &) const {
  // does nothing
}

void TestIdentityAction::read(char * &, char * &) {
  // does nothing
}

void TestIdentityAction::send_def(ToolCom *, DrawingLanguage) {
  // does nothing
}

// RaiseExceptionAction

RaiseExceptionAction::RaiseExceptionAction() {
}

RaiseExceptionAction::~RaiseExceptionAction() {
}

AnyAction * RaiseExceptionAction::duplicate() const {
  return new RaiseExceptionAction;
}

UmlActionKind RaiseExceptionAction::kind() const {
  return UmlRaiseExceptionAction;
}

QValueList<PinDescr> RaiseExceptionAction::pins() const {
  // [in] : exception
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlIn;
  p.name = "exception";
  r.append(p);

  return r;
}
  
bool RaiseExceptionAction::may_add_pin() const {
  return FALSE;
}

void RaiseExceptionAction::save(QTextStream &, QString &) const {
  // does nothing
}

void RaiseExceptionAction::read(char * &, char * &) {
  // does nothing
}

void RaiseExceptionAction::send_def(ToolCom *, DrawingLanguage) {
  // does nothing
}

// ReduceObjectAction

ReduceAction::ReduceAction()
    : is_ordered(FALSE), reducer(0) {
}

ReduceAction::~ReduceAction() {
}

AnyAction * ReduceAction::duplicate() const {
  ReduceAction * r = new ReduceAction;
  
  r->is_ordered = is_ordered;
  r->reducer = reducer;
  
  return r;
}

UmlActionKind ReduceAction::kind() const {
  return UmlReduceAction;
}

bool ReduceAction::may_add_pin() const {
  return FALSE;
}

QValueList<PinDescr> ReduceAction::pins() const {
  // [in] : collection
  // [out] : result
  QValueList<PinDescr> r;
  PinDescr p;

  p.dir = UmlIn;
  p.name = "collection";
  r.append(p);

  p.dir = UmlOut;
  p.name = "result";
  r.append(p);

  return r;
}

BasicData * ReduceAction::depend_on() {
  return (reducer) ? reducer->get_data() : 0;
}

void ReduceAction::on_delete() {
  reducer = 0;
}

BrowserNode * ReduceAction::referenced(const char *& s) const {
  if (reducer != 0)
    s = "reducer";
  return reducer;
}

void ReduceAction::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  
  if (is_ordered)
    st << "is_ordered ";
  
  if (reducer == 0)
    st << "no_reducer";
  else {
    switch (reducer->get_type()) {
    case UmlActivity:
      ((BrowserActivity *) reducer)->save(st, TRUE, warning);
      break;
    default:
      // a state machine
      ((BrowserState *) reducer)->save(st, TRUE, warning);
      break;
    }
  }
}

void ReduceAction::read(char * & st, char * & k) {
  if (!strcmp(k, "is_ordered")) {
    is_ordered = TRUE;
    k = read_keyword(st);
  }
  
  if (strcmp(k, "no_reducer")) {
    if (((reducer = BrowserActivity::read(st, k, 0)) == 0) &&
	((reducer = BrowserState::read(st, k, 0)) == 0))
      wrong_keyword(k, "no_reducer or activity_ref or state_ref");
  }
   
  k = read_keyword(st);
}

void ReduceAction::send_def(ToolCom * com, DrawingLanguage lang) {
  if (lang == UmlView) {
    com->write_bool(is_ordered);
    if (reducer != 0)
      reducer->write_id(com);
    else
      com->write_id(0);
  }
}

bool ReduceAction::tool_cmd(ToolCom * com, const char * args) {
  // note : write access already check
  switch ((unsigned char) args[-1]) {
  case setDefCmd:
    {
      BrowserNode * bn = (BrowserNode *) com->get_id(args);
      
      switch (bn->get_type()) {
      case UmlActivity:
      case UmlState:
	reducer = bn;
	break;
      default:
	return FALSE;
      }
    }
    break;
  case setFlagCmd:
    is_ordered = *args != 0;
    break;
  default:
    // cmd not managed at this level
    return FALSE;
  }
      
  // ok case
  return TRUE;
}
