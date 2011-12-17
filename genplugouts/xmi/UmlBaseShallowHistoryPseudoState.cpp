
#include "UmlBaseShallowHistoryPseudoState.h"
#include "UmlShallowHistoryPseudoState.h"
#include "UmlItem.h"

UmlShallowHistoryPseudoState * UmlBaseShallowHistoryPseudoState::create(UmlItem * parent)
{
  return (UmlShallowHistoryPseudoState *) parent->create_(aShallowHistoryPseudoState, "");
}

anItemKind UmlBaseShallowHistoryPseudoState::kind() {
  return aShallowHistoryPseudoState;
}

