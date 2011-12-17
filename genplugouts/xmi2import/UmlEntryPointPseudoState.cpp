
#include "UmlEntryPointPseudoState.h"

#include "UmlCom.h"
#include "FileIn.h"
void UmlEntryPointPseudoState::solve(QCString idref) {
  // note : parent state already resolved => is a state reference
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("state : unknown entry point reference '" + idref + "'<br>");
  }
  else if ((*it)->kind() == anEntryPointPseudoState)
    set_Reference((UmlEntryPointPseudoState *) *it);


}

