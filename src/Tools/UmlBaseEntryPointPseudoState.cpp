
#include "UmlBaseEntryPointPseudoState.h"
#include "UmlEntryPointPseudoState.h"
#include "UmlItem.h"

UmlEntryPointPseudoState * UmlBaseEntryPointPseudoState::create(UmlItem * parent, const char * s)
{
  return (UmlEntryPointPseudoState *) parent->create_(anEntryPointPseudoState, s);
}

anItemKind UmlBaseEntryPointPseudoState::kind() {
  return anEntryPointPseudoState;
}

void UmlBaseEntryPointPseudoState::read_uml_() {
  UmlBaseItem::read_uml_();
  (void) UmlBaseItem::read_();	// reference
}