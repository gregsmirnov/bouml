
#include "UmlBaseTerminatePseudoState.h"
#include "UmlTerminatePseudoState.h"
#include "UmlItem.h"

UmlTerminatePseudoState * UmlBaseTerminatePseudoState::create(UmlItem * parent)
{
  return (UmlTerminatePseudoState *) parent->create_(aTerminatePseudoState, "");
}

anItemKind UmlBaseTerminatePseudoState::kind() {
  return aTerminatePseudoState;
}

