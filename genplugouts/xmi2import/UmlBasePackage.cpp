
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
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

UmlPackage * UmlBasePackage::importProject(const QCString & fn) {
  UmlCom::send_cmd(_identifier, importProjectCmd, fn);

  return (UmlPackage *) UmlBaseItem::read_();
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

UmlPackage * UmlBasePackage::findCppNamespace(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findCppNamespaceCmd, _identifier, n);
  
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

UmlPackage * UmlBasePackage::findJavaPackage(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findJavaPackageCmd, _identifier, n);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}
#endif

#ifdef WITHPHP
const QCString & UmlBasePackage::phpDir() {
  read_if_needed_();
  
  return _php_dir;
}

bool UmlBasePackage::set_PhpDir(const QCString & s) {
  return set_it_(_php_dir, s, setPhpDirCmd);
}
const QCString & UmlBasePackage::phpNamespace() {
  read_if_needed_();

  return _php_namespace;
}

bool UmlBasePackage::set_PhpNamespace(const QCString & v) {
  return set_it_(_php_namespace, v, setPhpNamespaceCmd);
}

UmlPackage * UmlBasePackage::findPhpNamespace(const QCString & n) {
  UmlCom::send_cmd(packageGlobalCmd, findPhpNamespaceCmd, _identifier, n);

  return (UmlPackage *) UmlBaseItem::read_();
}
#endif

#ifdef WITHPYTHON
const QCString & UmlBasePackage::pythonDir() {
  read_if_needed_();
  
  return _python_dir;
}

bool UmlBasePackage::set_PythonDir(const QCString & s) {
  return set_it_(_python_dir, s, setPythonDirCmd);
}

QCString UmlBasePackage::pythonPackage() {
  read_if_needed_();
  
  return _python_package;
}

bool UmlBasePackage::set_PythonPackage(const QCString & s) {
  return set_it_(_python_package, s, setPythonPackageCmd);
}

UmlPackage * UmlBasePackage::findPythonPackage(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findPythonPackageCmd, _identifier, n);
  
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

UmlPackage * UmlBasePackage::findIdlModule(const QCString & n) const {
  UmlCom::send_cmd(packageGlobalCmd, findIdlModuleCmd, _identifier, n);
  
  return (UmlPackage *) UmlBaseItem::read_();  
}
#endif

UmlPackage * UmlBasePackage::getProject()
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

void UmlBasePackage::updateProfiles()
{
  UmlCom::send_cmd(packageGlobalCmd, updateProfileCmd);
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
#ifdef WITHPHP
  _php_dir = 0;
#endif
#ifdef WITHPYTHON
  _python_dir = 0;
  _python_package = 0;
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

#ifdef WITHPHP
void UmlBasePackage::read_php_() {
  _php_dir = UmlCom::read_string();
  _php_namespace = UmlCom::read_string();
}
#endif

#ifdef WITHPYTHON
void UmlBasePackage::read_python_() {
  _python_dir = UmlCom::read_string();
  _python_package = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBasePackage::read_idl_() {
  _idl_dir = UmlCom::read_string();
  _idl_module = UmlCom::read_string();
}
#endif

