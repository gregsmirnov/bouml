
#include "UmlBaseJoinPseudoState.h"
#include "UmlJoinPseudoState.h"
#include "UmlItem.h"

UmlJoinPseudoState * UmlBaseJoinPseudoState::create(UmlItem * parent)
{
  return (UmlJoinPseudoState *) parent->create_(aJoinPseudoState, "");
}

anItemKind UmlBaseJoinPseudoState::kind() {
  return aJoinPseudoState;
}

