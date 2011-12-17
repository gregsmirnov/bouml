
#include "UmlEntryPointPseudoState.h"
#include "UmlClass.h"
#include "UmlState.h"

#include "UmlCom.h"
#include "UmlTransition.h"
void UmlEntryPointPseudoState::generate(UmlClass * machine, UmlClass * anystate, UmlState * state, QCString & body, QCString indent) {
  // check transition number
  const QVector<UmlItem> ch = children();
  
  switch (ch.count()) {
  case 0:
    return;
  case 1:
    break;
  default:
    UmlCom::trace("Error : entry point '" + name() + "' can't have several transitions<br>");
    throw 0;
  }

  // here there is 1 transition
  ((UmlTransition *) ch[0])->generate(machine, anystate, state, body, indent);
}

