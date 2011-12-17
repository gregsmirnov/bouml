
#include "UmlBaseArtifact.h"
#include "UmlArtifact.h"
#include "UmlDeploymentView.h"
#include "UmlDeploymentDiagram.h"
#include "UmlClass.h"

#include "UmlCom.h"
UmlArtifact * UmlBaseArtifact::create(UmlDeploymentView * parent, const char * s)
{
  return (UmlArtifact *) parent->create_(anArtifact, s);
}

anItemKind UmlBaseArtifact::kind() {
  return anArtifact;
}

UmlDeploymentDiagram * UmlBaseArtifact::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBaseArtifact::set_AssociatedDiagram(UmlDeploymentDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

const QVector<UmlClass> & UmlBaseArtifact::associatedClasses() {
  read_if_needed_();
  
  return _assoc_classes;
}

bool UmlBaseArtifact::addAssociatedClass(UmlClass * cl) {
  UmlCom::send_cmd(_identifier, addAssocClassCmd,
		   ((UmlBaseClass *) cl)->_identifier);
  if (UmlCom::read_bool()) {
    if (_defined) {
      _assoc_classes.resize(_assoc_classes.size() + 1);
      _assoc_classes.insert(_assoc_classes.size() - 1, cl);
    }
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseArtifact::removeAssociatedClass(UmlClass * cl) {
  UmlCom::send_cmd(_identifier, removeAssocClassCmd,
		   ((UmlBaseClass *) cl)->_identifier);
  if (UmlCom::read_bool()) {
    if (_defined) {
      unsigned index = (unsigned) _assoc_classes.findRef(cl);
      
      if (((int) index) == -1)
	// theo impossible
	return FALSE;
      
      unsigned last = _assoc_classes.size() - 1;
      
      if (index != last)
	_assoc_classes.insert(index, _assoc_classes[last]);
      
      _assoc_classes.resize(last);
    }
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseArtifact::set_AssociatedClasses(const QVector<UmlClass> & l) {
  UmlCom::send_cmd(_identifier, setAssocClassesCmd, (const QVector<UmlItem> &) l);
  if (UmlCom::read_bool()) {
      // tests != to bypass Qt 2.3 bug
    if (_defined && (&_assoc_classes != &l))
      _assoc_classes = l;
    return TRUE;
  }
  else
    return FALSE;
}

const QVector<UmlArtifact> & UmlBaseArtifact::associatedArtifacts() {
  read_if_needed_();
  
  return _associated;
}

bool UmlBaseArtifact::addAssociatedArtifact(UmlArtifact * cp) {
  UmlCom::send_cmd(_identifier, addAssocArtifactCmd, 
		   ((UmlBaseArtifact *) cp)->_identifier);
  if (UmlCom::read_bool()) {
    if (_defined) {
      _associated.resize(_associated.size() + 1);
      _associated.insert(_associated.size() - 1, cp);
    }
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseArtifact::removeAssociatedArtifact(UmlArtifact * cp) {
  UmlCom::send_cmd(_identifier, removeAssocArtifactCmd, 
		   ((UmlBaseArtifact *) cp)->_identifier);
  if (UmlCom::read_bool()) {
    if (_defined) {
      unsigned index = (unsigned) _associated.findRef(cp);
      
      if (((int) index) == -1)
	// theo impossible
	return FALSE;
      
      unsigned last = _associated.size() - 1;
      
      if (index != last)
	_associated.insert(index, _associated[last]);
      
      _associated.resize(last);
    }
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseArtifact::removeAllAssociatedArtifacts() {
  UmlCom::send_cmd(_identifier, removeAllAssocArtifactsCmd);
  if (UmlCom::read_bool()) {  
    _associated.clear();
    return TRUE;
  }
  else
    return FALSE;
}

#ifdef WITHCPP
const QCString & UmlBaseArtifact::cppHeader() {
  read_if_needed_();
  
  return _cpp_h;
}

bool UmlBaseArtifact::set_CppHeader(const QCString & s) {
  return set_it_(_cpp_h, s, setCppHCmd);
}

const QCString & UmlBaseArtifact::cppSource() {
  read_if_needed_();
  
  return _cpp_src;
}

bool UmlBaseArtifact::set_CppSource(const QCString & s) {
  return set_it_(_cpp_src, s, setCppSrcCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseArtifact::javaSource() {
  read_if_needed_();
  
  return _java_src;
}

bool UmlBaseArtifact::set_JavaSource(const QCString & s) {
  return set_it_(_java_src, s, setJavaSrcCmd);
}
#endif

#ifdef WITHPHP
const QCString & UmlBaseArtifact::phpSource() {
  read_if_needed_();
  
  return _php_src;
}

bool UmlBaseArtifact::set_PhpSource(const QCString & s) {
  return set_it_(_php_src, s, setPhpSrcCmd);
}
#endif

#ifdef WITHPYTHON
const QCString & UmlBaseArtifact::pythonSource() {
  read_if_needed_();
  
  return _python_src;
}

bool UmlBaseArtifact::set_PythonSource(const QCString & s) {
  return set_it_(_python_src, s, setPythonSrcCmd);
}
#endif

#ifdef WITHIDL
const QCString & UmlBaseArtifact::idlSource() {
  read_if_needed_();
  
  return _idl_src;
}

bool UmlBaseArtifact::set_IdlSource(const QCString & s) {
  return set_it_(_idl_src, s, setIdlSrcCmd);
}
#endif

void UmlBaseArtifact::unload(bool rec, bool del) {
  _assoc_classes.clear();
  _associated.clear();
#ifdef WITHCPP
  _cpp_h = 0;
  _cpp_src = 0;
#endif
#ifdef WITHJAVA
  _java_src = 0;
#endif
#ifdef WITHPHP
  _php_src = 0;
#endif
#ifdef WITHPYTHON
  _python_src = 0;
#endif
#ifdef WITHIDL
  _idl_src = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

void UmlBaseArtifact::read_uml_() {
  _assoc_diagram = (UmlDeploymentDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  _assoc_classes.resize(n);
    
  for (index = 0; index != n; index += 1)
    _assoc_classes.insert(index, (UmlClass *) UmlBaseItem::read_());
  
  n = UmlCom::read_unsigned();
  _associated.resize(n);
    
  for (index = 0; index != n; index += 1)
    _associated.insert(index, (UmlArtifact *) UmlBaseItem::read_());
}

#ifdef WITHCPP
void UmlBaseArtifact::read_cpp_() {
  _cpp_h = UmlCom::read_string();
  _cpp_src = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseArtifact::read_java_() {
  _java_src = UmlCom::read_string();
}
#endif

#ifdef WITHPHP
void UmlBaseArtifact::read_php_() {
  _php_src = UmlCom::read_string();
}
#endif

#ifdef WITHPYTHON
void UmlBaseArtifact::read_python_() {
  _python_src = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBaseArtifact::read_idl_() {
  _idl_src = UmlCom::read_string();
}
#endif

