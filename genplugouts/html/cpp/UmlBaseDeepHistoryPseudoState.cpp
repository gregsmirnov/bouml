
#include "UmlBaseDeepHistoryPseudoState.h"
#include "UmlDeepHistoryPseudoState.h"
#include "UmlItem.h"

UmlDeepHistoryPseudoState * UmlBaseDeepHistoryPseudoState::create(UmlItem * parent)
{
  return (UmlDeepHistoryPseudoState *) parent->create_(aDeepHistoryPseudoState, "");
}

anItemKind UmlBaseDeepHistoryPseudoState::kind() {
  return aDeepHistoryPseudoState;
}

