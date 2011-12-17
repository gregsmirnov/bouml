
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

