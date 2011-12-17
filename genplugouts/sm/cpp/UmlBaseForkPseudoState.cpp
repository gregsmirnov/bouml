
#include "UmlBaseForkPseudoState.h"
#include "UmlForkPseudoState.h"
#include "UmlItem.h"

UmlForkPseudoState * UmlBaseForkPseudoState::create(UmlItem * parent)
{
  return (UmlForkPseudoState *) parent->create_(aForkPseudoState, "");
}

anItemKind UmlBaseForkPseudoState::kind() {
  return aForkPseudoState;
}

