
#include "UmlCom.h"
#include "UmlBaseActivityObject.h"
#include "UmlActivityObject.h"
#include "UmlItem.h"
#include "UmlActivityDiagram.h"

UmlActivityObject * UmlBaseActivityObject::create(UmlItem * parent, const char * s)
{
  return (UmlActivityObject *) parent->create_(anActivityObject, s);
}

anItemKind UmlBaseActivityObject::kind() {
  return anActivityObject;
}

const UmlTypeSpec & UmlBaseActivityObject::type() {
  read_if_needed_();
  return _type;
}

bool UmlBaseActivityObject::set_Type(const UmlTypeSpec & v) {
  return set_it_(_type, v, setTypeCmd);
}

const QCString & UmlBaseActivityObject::multiplicity() {
  read_if_needed_();
  return _multiplicity;
}

bool UmlBaseActivityObject::set_Multiplicity(const char * v) {
  return set_it_(_multiplicity, v, setMultiplicityCmd);
}

const QCString & UmlBaseActivityObject::inState() {
  read_if_needed_();
  return _in_state;
}

bool UmlBaseActivityObject::set_InState(const char * v) {
  return set_it_(_in_state, v, setInStateCmd);
}

const QCString & UmlBaseActivityObject::selection() {
  read_if_needed_();
  return _selection;
}

bool UmlBaseActivityObject::set_Selection(const char * v) {
  return set_it_(_selection, v, setUmlActivityCmd);
}

#ifdef WITHCPP
const QCString & UmlBaseActivityObject::cppSelection() {
  read_if_needed_();
  return _cpp_selection;
}

bool UmlBaseActivityObject::set_CppSelection(const char * v) {
  return set_it_(_cpp_selection, v, setCppActivityCmd);
}
#endif

#ifdef WITHJAVA
const QCString & UmlBaseActivityObject::javaSelection() {
  read_if_needed_();
  return _java_selection;
}

bool UmlBaseActivityObject::set_JavaSelection(const char * v) {
  return set_it_(_java_selection, v, setJavaActivityCmd);
}
#endif

anOrdering UmlBaseActivityObject::ordering() {
  read_if_needed_();
  return _ordering;
}

bool UmlBaseActivityObject::set_Ordering(anOrdering v) {
  UmlCom::send_cmd(_identifier, setOrderingCmd, (char) v);   if (UmlCom::read_bool()) {     _ordering = v;     return TRUE;   }   else     return FALSE;
}

bool UmlBaseActivityObject::isControlType() {
  read_if_needed_();
  return _is_control;
}

bool UmlBaseActivityObject::set_IsControlType(bool v) {
  bool vv;

  if (set_it_(vv, v, setFlagCmd)) {
    _is_control = v;
    return TRUE;
  }
  else
    return FALSE;
}

UmlActivityDiagram * UmlBaseActivityObject::associatedDiagram() {
  read_if_needed_();

  return _assoc_diagram;
}

bool UmlBaseActivityObject::set_AssociatedDiagram(UmlActivityDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseActivityObject::unload(bool rec, bool del) {
  _type.explicit_type = 0;
  _multiplicity = 0;
  _in_state = 0;
  _selection = 0;
#ifdef WINCPP
  _cpp_selection = 0;
#endif
#ifdef WITHJAVA
  _java_selection = 0;
#endif
  UmlBaseActivityNode::unload(rec, del);
}

void UmlBaseActivityObject::read_uml_() {
  _assoc_diagram = (kind() == anActivityObject) ? (UmlActivityDiagram *) UmlBaseItem::read_() : (UmlActivityDiagram *) 0;
  UmlBaseActivityNode::read_uml_();
  _type.type = (UmlClass *) UmlBaseItem::read_();
  if (_type.type == 0)
    _type.explicit_type = UmlCom::read_string();
  _multiplicity = UmlCom::read_string();
  _in_state = UmlCom::read_string();
  _selection = UmlCom::read_string();
  _ordering = (anOrdering) UmlCom::read_char();
  _is_control = UmlCom::read_bool();
}

#ifdef WITHCPP
void UmlBaseActivityObject::read_cpp_() {
  _cpp_selection = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseActivityObject::read_java_() {
  _java_selection = UmlCom::read_string();
}
#endif

