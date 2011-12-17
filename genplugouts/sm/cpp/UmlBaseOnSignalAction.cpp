
#include "UmlCom.h"
#include "UmlBaseOnSignalAction.h"

const QCString & UmlBaseOnSignalAction::signal() {
  read_if_needed_();
  return _signal;
}

bool UmlBaseOnSignalAction::set_Signal(const char * v) {
  return set_it_(_signal, v, setUmlActivityCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseOnSignalAction::cppSignal() {
  read_if_needed_();
  return _cpp_signal;
}

bool UmlBaseOnSignalAction::set_CppSignal(const char * v) {
  return set_it_(_cpp_signal, v, setCppActivityCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseOnSignalAction::javaSignal() {
  read_if_needed_();
  return _java_signal;
}

bool UmlBaseOnSignalAction::set_JavaSignal(const char * v) {
  return set_it_(_java_signal, v, setJavaActivityCmd);
}
#endif

void UmlBaseOnSignalAction::unload(bool rec, bool del) {
  _signal = 0;
#ifdef WITHCPP
  _cpp_signal = 0;
#endif
#ifdef WITHJAVA
  _java_signal = 0;
#endif
  UmlBaseActivityAction::unload(rec, del);
}

void UmlBaseOnSignalAction::read_uml_() {
  UmlBaseActivityAction::read_uml_();
  _signal = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseOnSignalAction::read_cpp_() {
  UmlBaseActivityAction::read_cpp_();
  _cpp_signal = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseOnSignalAction::read_java_() {
  UmlBaseActivityAction::read_java_();
  _java_signal = UmlCom::read_string();
}
#endif

