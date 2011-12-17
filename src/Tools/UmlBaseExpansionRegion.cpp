
#include "UmlCom.h"
#include "UmlBaseExpansionRegion.h"
#include "UmlExpansionRegion.h"
#include "UmlItem.h"

UmlExpansionRegion * UmlBaseExpansionRegion::create(UmlItem * parent, const char * s)
{
  return (UmlExpansionRegion *) parent->create_(anExpansionRegion, s);
}

anItemKind UmlBaseExpansionRegion::kind() {
  return anExpansionRegion;
}

bool UmlBaseExpansionRegion::isMustIsolate() {
  read_if_needed_();
  return _must_isolate;
}

bool UmlBaseExpansionRegion::set_isMustIsolate(bool v) {
  return set_it_(_must_isolate, v, setFlagCmd);
}

anExpansionKind UmlBaseExpansionRegion::mode() {
  read_if_needed_();
  return _mode;
}

bool UmlBaseExpansionRegion::set_Mode(anExpansionKind v) {
  UmlCom::send_cmd(_identifier, setDefCmd, (char) v);   if (UmlCom::read_bool()) {     _mode = v;     return TRUE;   }   else     return FALSE;
}

void UmlBaseExpansionRegion::read_uml_() {
  UmlBaseActivityRegion::read_uml_();
  _must_isolate = UmlCom::read_bool();
  _mode = (anExpansionKind) UmlCom::read_char();
}

