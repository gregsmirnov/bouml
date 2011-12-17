
#include "UmlInitialPseudoState.h"
#include "UmlClass.h"

#include "UmlCom.h"
#include "UmlState.h"
#include "UmlTransition.h"
#include "UmlOperation.h"

void UmlInitialPseudoState::generate(UmlClass * machine, UmlClass * anystate, UmlState * state) {
  // checking

  state->hasInitial();
  
  const QVector<UmlItem> ch = children();
  
  if (ch.count() != 1) {
    UmlCom::trace("Error : 'initial' pseudo state must have one and only one transition<br>");
    throw 0;
  }

  UmlTransition * tr = (UmlTransition *) ch[0];
  
  if (tr->target()->parent() != parent()) {
    UmlCom::trace("Error : transition from 'initial' must goes to a sub [pseudo] state of the state '"
		  + state->name() + "'<br>");
    throw 0;
  }

  if (!tr->cppGuard().isEmpty()) {
    UmlCom::trace("Error : transition from 'initial' must have no guard<br>");
    throw 0;
  }

  tr->generate(machine, anystate, state);
}

