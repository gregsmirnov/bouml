
#include "UmlBaseStateDiagram.h"
#include "UmlStateDiagram.h"
#include "UmlState.h"

UmlStateDiagram * UmlBaseStateDiagram::create(UmlState * parent, const char * s)
{
  return (UmlStateDiagram *) parent->create_(aStateDiagram, s);
}

anItemKind UmlBaseStateDiagram::kind() {
  return aStateDiagram;
}

