
#include "UmlExitPointPseudoState.h"

#include "UmlCom.h"
#include "FileIn.h"
void UmlExitPointPseudoState::solve(QCString idref) {
  // note : parent state already resolved => is a state reference
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("state : unknown exit point reference '" + idref + "'<br>");
  }
  else if ((*it)->kind() == anExitPointPseudoState)
    set_Reference((UmlExitPointPseudoState *) *it);


}

