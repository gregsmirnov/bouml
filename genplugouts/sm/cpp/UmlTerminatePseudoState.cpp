
#include "UmlTerminatePseudoState.h"
#include "UmlClass.h"
#include "UmlState.h"

void UmlTerminatePseudoState::generate(UmlClass *, UmlClass *, UmlState *, QCString & body, QCString indent) {
  body += indent + "stm._final();\n";
}

