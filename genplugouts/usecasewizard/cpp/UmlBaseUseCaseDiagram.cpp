
#include "UmlBaseUseCaseDiagram.h"
#include "UmlUseCaseDiagram.h"
#include "UmlItem.h"

UmlUseCaseDiagram * UmlBaseUseCaseDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlUseCaseDiagram *) parent->create_(anUseCaseDiagram, s);
}

anItemKind UmlBaseUseCaseDiagram::kind() {
  return anUseCaseDiagram;
}

