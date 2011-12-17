
#include "UmlCom.h"
#include "UmlBaseEntryPointPseudoState.h"
#include "UmlEntryPointPseudoState.h"
#include "UmlItem.h"

UmlEntryPointPseudoState * UmlBaseEntryPointPseudoState::create(UmlItem * parent, const char * s)
{
  return (UmlEntryPointPseudoState *) parent->create_(anEntryPointPseudoState, s);
}

anItemKind UmlBaseEntryPointPseudoState::kind() {
  return anEntryPointPseudoState;
}

UmlEntryPointPseudoState * UmlBaseEntryPointPseudoState::reference() {
  read_if_needed_();
  return _reference;
}

bool UmlBaseEntryPointPseudoState::set_Reference(UmlEntryPointPseudoState * v) {
  UmlCom::send_cmd(_identifier, setDerivedCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
  if (UmlCom::read_bool()) {
    _reference = v;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseEntryPointPseudoState::read_uml_() {
  UmlBaseItem::read_uml_();
  _reference = (UmlEntryPointPseudoState *) UmlBaseItem::read_();
}

