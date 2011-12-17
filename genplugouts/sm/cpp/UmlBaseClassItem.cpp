
#include "UmlBaseClassItem.h"

#include "UmlCom.h"
#ifdef WITHCPP
const QCString & UmlBaseClassItem::cppDecl() {
  read_if_needed_();
  
  return _cpp_decl;
}

bool UmlBaseClassItem::set_CppDecl(const char * s) {
  return set_it_(_cpp_decl, s, setCppDeclCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseClassItem::javaDecl() {
  read_if_needed_();
  
  return _java_decl;
}

bool UmlBaseClassItem::set_JavaDecl(const char * s) {
  return set_it_(_java_decl, s, setJavaDeclCmd);
}
#endif

#ifdef WITHPHP
const QCString & UmlBaseClassItem::phpDecl() {
  read_if_needed_();
  
  return _php_decl;
}

bool UmlBaseClassItem::set_PhpDecl(const char * s) {
  return set_it_(_php_decl, s, setPhpDeclCmd);
}
#endif

#ifdef WITHIDL
const QCString & UmlBaseClassItem::idlDecl() {
  read_if_needed_();
  
  return _idl_decl;
}

bool UmlBaseClassItem::set_IdlDecl(const char * s) {
  return set_it_(_idl_decl, s, setIdlDeclCmd);
}
#endif

void UmlBaseClassItem::unload(bool rec, bool del) {
#ifdef WITHCPP
  _cpp_decl = 0;
#endif
#ifdef WITHJAVA
  _java_decl = 0;
#endif
#ifdef WITHPHP
  _php_decl = 0;
#endif
#ifdef WITHIDL
  _idl_decl = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

#ifdef WITHCPP
void UmlBaseClassItem::read_cpp_() {
  _cpp_decl = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseClassItem::read_java_() {
  _java_decl = UmlCom::read_string();
}
#endif

#ifdef WITHPHP
void UmlBaseClassItem::read_php_() {
  _php_decl = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBaseClassItem::read_idl_() {
  _idl_decl = UmlCom::read_string();
}
#endif

