
#include "UmlBaseInitialPseudoState.h"
#include "UmlInitialPseudoState.h"
#include "UmlItem.h"

UmlInitialPseudoState * UmlBaseInitialPseudoState::create(UmlItem * parent)
{
  return (UmlInitialPseudoState *) parent->create_(anInitialPseudoState, "");
}

anItemKind UmlBaseInitialPseudoState::kind() {
  return anInitialPseudoState;
}

