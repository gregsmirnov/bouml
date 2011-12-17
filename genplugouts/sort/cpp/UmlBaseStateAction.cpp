
#include "UmlCom.h"
#include "UmlBaseStateAction.h"
#include "UmlStateAction.h"
#include "UmlState.h"

UmlStateAction * UmlBaseStateAction::create(UmlState * parent)
{
  return (UmlStateAction *) parent->create_(aStateAction, "");
}

anItemKind UmlBaseStateAction::kind() {
  return aStateAction;
}

const QCString & UmlBaseStateAction::expression() {
  read_if_needed_();
  return _uml;
}

bool UmlBaseStateAction::set_Expression(const char * s) {
  return set_it_(_uml, s, setDefCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseStateAction::cppExpression() {
  read_if_needed_();
  return _cpp;
}

bool UmlBaseStateAction::set_CppExpression(const char * s) {
  return set_it_(_cpp, s, setCppDeclCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseStateAction::javaExpression() {
  read_if_needed_();
  return _java;
}

bool UmlBaseStateAction::set_JavaExpression(const char * s) {
  return set_it_(_java, s, setJavaDeclCmd);
}
#endif

void UmlBaseStateAction::unload(bool rec, bool del) {
  _uml = 0;
#ifdef WITHCPP
  _cpp = 0;
#endif
#ifdef WITHJAVA
  _java = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

void UmlBaseStateAction::read_uml_() {
  UmlBaseItem::read_uml_();
  _uml = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseStateAction::read_cpp_() {
  _cpp = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseStateAction::read_java_() {
  _java = UmlCom::read_string();
}
#endif

