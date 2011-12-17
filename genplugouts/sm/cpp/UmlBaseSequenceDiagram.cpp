
#include "UmlBaseSequenceDiagram.h"
#include "UmlSequenceDiagram.h"
#include "UmlItem.h"

UmlSequenceDiagram * UmlBaseSequenceDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlSequenceDiagram *) parent->create_(aSequenceDiagram, s);
}

anItemKind UmlBaseSequenceDiagram::kind() {
  return aSequenceDiagram;
}

