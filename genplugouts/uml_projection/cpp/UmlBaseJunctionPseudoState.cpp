
#include "UmlBaseJunctionPseudoState.h"
#include "UmlJunctionPseudoState.h"
#include "UmlItem.h"

UmlJunctionPseudoState * UmlBaseJunctionPseudoState::create(UmlItem * parent)
{
  return (UmlJunctionPseudoState *) parent->create_(aJunctionPseudoState, "");
}

anItemKind UmlBaseJunctionPseudoState::kind() {
  return aJunctionPseudoState;
}

