
#include "UmlBaseChoicePseudoState.h"
#include "UmlChoicePseudoState.h"
#include "UmlItem.h"

UmlChoicePseudoState * UmlBaseChoicePseudoState::create(UmlItem * parent)
{
  return (UmlChoicePseudoState *) parent->create_(aChoicePseudoState, "");
}

anItemKind UmlBaseChoicePseudoState::kind() {
  return aChoicePseudoState;
}

