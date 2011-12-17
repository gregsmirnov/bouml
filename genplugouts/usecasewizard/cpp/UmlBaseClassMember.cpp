
#include "UmlBaseClassMember.h"

#include "UmlCom.h"
bool UmlBaseClassMember::isClassMember() {
  read_if_needed_();
  
  return _class_member;
}

bool UmlBaseClassMember::set_isClassMember(bool y) {
  return set_it_(_class_member, y, setIsClassMemberCmd);
}

bool UmlBaseClassMember::isVolatile() {
  read_if_needed_();
  
  return _volatile;
}

bool UmlBaseClassMember::set_isVolatile(bool y) {
  return set_it_(_volatile, y, setIsVolatileCmd);
}


aVisibility UmlBaseClassMember::visibility() {
  read_if_needed_();
  
  return _visibility;
}

bool UmlBaseClassMember::set_Visibility(aVisibility v) {
  aVisibility vi;
  
  if ((v != DefaultVisibility) && set_it_(vi, v, setVisibilityCmd)) {
    _visibility = v;
    return TRUE;
  }
  else
    return FALSE;
}

#ifdef WITHCPP
aVisibility UmlBaseClassMember::cppVisibility() {
  read_if_needed_();
  
  return _cpp_visibility;
}

bool UmlBaseClassMember::set_CppVisibility(aVisibility v) {
  aVisibility vi;
  
  if (set_it_(vi, v, setCppVisibilityCmd)) {
    _cpp_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
}
#endif

void UmlBaseClassMember::read_uml_() {
  UmlBaseItem::read_uml_();
  _class_member = UmlCom::read_bool();
  _volatile = UmlCom::read_bool();
  _visibility = (aVisibility) UmlCom::read_char();
}

#ifdef WITHCPP
void UmlBaseClassMember::read_cpp_() {
  UmlBaseClassItem::read_cpp_();
  _cpp_visibility = (aVisibility) UmlCom::read_char();
}
#endif

