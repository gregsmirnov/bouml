
#include "UmlCom.h"
#include "UmlBaseTransition.h"
#include "UmlTransition.h"

anItemKind UmlBaseTransition::kind() {
  return aTransition;
}

UmlTransition * UmlBaseTransition::create(UmlItem * start, UmlItem * end)
{
  UmlCom::send_cmd(start->_identifier, createCmd, aTransition,
		   end->_identifier);
  UmlTransition * result = (UmlTransition *) UmlBaseItem::read_();

  if (result != 0)
    start->reread_children_if_needed_();
  return result;
}

UmlItem * UmlBaseTransition::target() {
  read_if_needed_();
  return _target;
}

const QCString & UmlBaseTransition::trigger() {
  read_if_needed_();
  return _uml.trigger;
}

bool UmlBaseTransition::set_Trigger(const char * s) {
  return set_it_(_uml.trigger, s, setUmlTriggerCmd);
}

const QCString & UmlBaseTransition::guard() {
  read_if_needed_();
  return _uml.guard;
}

bool UmlBaseTransition::set_Guard(const char * s) {
  return set_it_(_uml.guard, s, setUmlGuardCmd);
}

const QCString & UmlBaseTransition::activity() {
  read_if_needed_();
  return _uml.activity;
}

bool UmlBaseTransition::set_Activity(const char * s) {
  return set_it_(_uml.activity, s, setUmlActivityCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseTransition::cppTrigger() {
  read_if_needed_();
  return _cpp.trigger;
}

bool UmlBaseTransition::set_CppTrigger(const char * s) {
  return set_it_(_cpp.trigger, s, setCppTriggerCmd);
}

const QCString & UmlBaseTransition::cppGuard() {
  read_if_needed_();
  return _cpp.guard;
}

bool UmlBaseTransition::set_CppGuard(const char * s) {
  return set_it_(_cpp.guard, s, setCppGuardCmd);
}

const QCString & UmlBaseTransition::cppActivity() {
  read_if_needed_();
  return _cpp.activity;
}

bool UmlBaseTransition::set_CppActivity(const char * s) {
  return set_it_(_cpp.activity, s, setCppActivityCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseTransition::javaTrigger() {
  read_if_needed_();
  return _java.trigger;
}

bool UmlBaseTransition::set_JavaTrigger(const char * s) {
  return set_it_(_java.trigger, s, setJavaTriggerCmd);
}

const QCString & UmlBaseTransition::javaGuard() {
  read_if_needed_();
  return _java.guard;
}

bool UmlBaseTransition::set_JavaGuard(const char * s) {
  return set_it_(_java.guard, s, setJavaGuardCmd);
}

const QCString & UmlBaseTransition::javaActivity() {
  read_if_needed_();
  return _java.activity;
}

bool UmlBaseTransition::set_JavaActivity(const char * s) {
  return set_it_(_java.activity, s, setJavaActivityCmd);
}
#endif

void UmlBaseTransition::unload(bool rec, bool del) {
  _uml.unload();
#ifdef WITHCPP
  _cpp.unload();
#endif
#ifdef WITHJAVA
  _java.unload();
#endif
  UmlBaseItem::unload(rec, del);
}

void UmlBaseTransition::read_uml_() {
  UmlBaseItem::read_uml_();
  _target = UmlBaseItem::read_();
  _uml.read();
}

#ifdef WITHCPP
void UmlBaseTransition::read_cpp_() {
  _cpp.read();
}
#endif

#ifdef WITHJAVA
void UmlBaseTransition::read_java_() {
  _java.read();
}
#endif

