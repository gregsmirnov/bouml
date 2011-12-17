
#include "UmlBaseActivityDiagram.h"
#include "UmlActivityDiagram.h"
#include "UmlItem.h"

UmlActivityDiagram * UmlBaseActivityDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlActivityDiagram *) parent->create_(anActivityDiagram, s);
}

anItemKind UmlBaseActivityDiagram::kind() {
  return anActivityDiagram;
}

