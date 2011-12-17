
#include "UmlBaseExitPointPseudoState.h"
#include "UmlExitPointPseudoState.h"
#include "UmlItem.h"

UmlExitPointPseudoState * UmlBaseExitPointPseudoState::create(UmlItem * parent, const char * s)
{
  return (UmlExitPointPseudoState *) parent->create_(anExitPointPseudoState, s);
}

anItemKind UmlBaseExitPointPseudoState::kind() {
  return anExitPointPseudoState;
}

void UmlBaseExitPointPseudoState::read_uml_() {
  UmlBaseItem::read_uml_();
  (void) UmlBaseItem::read_();	// reference
}
