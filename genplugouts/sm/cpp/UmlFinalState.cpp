
#include "UmlFinalState.h"
#include "UmlClass.h"
#include "UmlState.h"

#include "UmlCom.h"
#include "UmlOperation.h"
#include "UmlTransition.h"

void UmlFinalState::generate(UmlClass *, UmlClass *, UmlState *, QCString & body, QCString indent) {
  if (parent()->kind() != aRegion)
    body += indent + "stm._final();\n";
  else {
#ifndef WIN32
#warning A FAIRE
#endif
  }
}

