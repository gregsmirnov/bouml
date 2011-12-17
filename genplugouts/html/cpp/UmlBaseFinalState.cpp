
#include "UmlBaseFinalState.h"
#include "UmlFinalState.h"
#include "UmlItem.h"

UmlFinalState * UmlBaseFinalState::create(UmlItem * parent)
{
  return (UmlFinalState *) parent->create_(aFinalState, "");
}

anItemKind UmlBaseFinalState::kind() {
  return aFinalState;
}

