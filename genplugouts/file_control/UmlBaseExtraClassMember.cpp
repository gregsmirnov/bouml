
#include "UmlBaseExtraClassMember.h"
#include "UmlExtraClassMember.h"
#include "UmlClass.h"

#include "UmlCom.h"
UmlExtraClassMember * UmlBaseExtraClassMember::create(UmlClass * parent, const char * name)
{
  return (UmlExtraClassMember *) parent->create_(anExtraClassMember, name);
}

anItemKind UmlBaseExtraClassMember::kind() {
  return anExtraClassMember;
}

#ifdef WITHCPP
bool UmlBaseExtraClassMember::isCppInline() {
  read_if_needed_();
    
  return _cpp_inline;
}

bool UmlBaseExtraClassMember::set_isCppInline(bool y) {
  return set_it_(_cpp_inline, y, setIsCppInlineCmd);
}

const QCString & UmlBaseExtraClassMember::cppDef() {
  read_if_needed_();
    
  return _cpp_def;
}

bool UmlBaseExtraClassMember::set_CppDef(const char * s) {
  return set_it_(_cpp_def, s, setCppDefCmd);
}
#endif

void UmlBaseExtraClassMember::unload(bool rec, bool del) {
#ifdef WITHCPP
  _cpp_def = 0;
#endif
  UmlBaseClassItem::unload(rec, del);
}

#ifdef WITHCPP
void UmlBaseExtraClassMember::read_cpp_() {
  UmlBaseClassItem::read_cpp_();
  _cpp_def = UmlCom::read_string();
  _cpp_inline = UmlCom::read_bool();
}
#endif

