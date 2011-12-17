
#include "UmlBasePackage.h"
#include "UmlPackage.h"
#include "UmlDiagram.h"

#include "UmlCom.h"
#include "PackageGlobalCmd.h"
#include "MiscGlobalCmd.h"
UmlPackage * UmlBasePackage::create(UmlPackage * parent, const char * name)
{
  return (UmlPackage *) parent->create_(aPackage, name);
}

anItemKind UmlBasePackage::kind() {
  return aPackage;
}

UmlDiagram * UmlBasePackage::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBasePackage::set_AssociatedDiagram(UmlDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

#ifdef WITHCPP
const QCString & UmlBasePackage::cppSrcDir() {
  read_if_needed_();
  
  return _cpp_src_dir;
}

bool UmlBasePackage::set_CppSrcDir(const QCString & s) {
  return set_it_(_cpp_src_dir, s, setCppSrcDirCmd);
}

const QCString & UmlBasePackage::cppHDir() {
  read_if_needed_();
  
  return _cpp_h_dir;
}

bool UmlBasePackage::set_CppHDir(const QCString & s) {
  return set_it_(_cpp_h_dir, s, setCppHDirCmd);
}

QCString UmlBasePackage::cppNamespace() {
  read_if_needed_();
  
  return _cpp_namespace;
}

bool UmlBasePackage::set_CppNamespace(const QCString & s) {
  return set_it_(_cpp_namespace, s, setCppNamespaceCmd);
}

UmlPackage * UmlBasePackage::findNamespace(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findNamespaceCmd, _identifier, n);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}
#endif

#ifdef WITHJAVA
const QCString & UmlBasePackage::javaDir() {
  read_if_needed_();
  
  return _java_dir;
}

bool UmlBasePackage::set_JavaDir(const QCString & s) {
  return set_it_(_java_dir, s, setJavaDirCmd);
}

QCString UmlBasePackage::javaPackage() {
  read_if_needed_();
  
  return _java_package;
}

bool UmlBasePackage::set_JavaPackage(const QCString & s) {
  return set_it_(_java_package, s, setJavaPackageCmd);
}

UmlPackage * UmlBasePackage::findPackage(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findPackageCmd, _identifier, n);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}
#endif

#ifdef WITHIDL
const QCString & UmlBasePackage::idlDir() {
  read_if_needed_();
  
  return _idl_dir;
}

bool UmlBasePackage::set_IdlDir(const QCString & s) {
  return set_it_(_idl_dir, s, setIdlDirCmd);
}

QCString UmlBasePackage::idlModule() {
  read_if_needed_();
  
  return _idl_module;
}

bool UmlBasePackage::set_IdlModule(const QCString & s) {
  return set_it_(_idl_module, s, setIdlModuleCmd);
}

UmlPackage * UmlBasePackage::findModule(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findModuleCmd, _identifier, n);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}
#endif

UmlBasePackage * UmlBasePackage::getProject()
{
  UmlCom::send_cmd(packageGlobalCmd, getProjectCmd);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}

bool UmlBasePackage::isProjectModified()
{
  UmlCom::send_cmd(packageGlobalCmd, isProjectModifiedCmd);
  
  return UmlCom::read_bool();
}

void UmlBasePackage::saveProject()
{
  UmlCom::send_cmd(packageGlobalCmd, saveProjectCmd);
}

void UmlBasePackage::loadProject(QCString p)
{
  UmlCom::send_cmd(miscGlobalCmd, loadCmd, (const char *) p);
}

void UmlBasePackage::unload(bool rec, bool del) {
  _assoc_diagram = 0;
#ifdef WITHCPP
  _cpp_src_dir = 0;
  _cpp_h_dir = 0;
  _cpp_namespace = 0;
#endif
#ifdef WITHJAVA
  _java_dir = 0;
  _java_package = 0;
#endif
#ifdef WITHIDL
  _idl_dir = 0;
  _idl_module = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

void UmlBasePackage::read_uml_() {
  _assoc_diagram = (UmlDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
}

#ifdef WITHCPP
void UmlBasePackage::read_cpp_() {
  _cpp_src_dir = UmlCom::read_string();
  _cpp_h_dir = UmlCom::read_string();
  _cpp_namespace = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBasePackage::read_java_() {
  _java_dir = UmlCom::read_string();
  _java_package = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBasePackage::read_idl_() {
  _idl_dir = UmlCom::read_string();
  _idl_module = UmlCom::read_string();
}
#endif

