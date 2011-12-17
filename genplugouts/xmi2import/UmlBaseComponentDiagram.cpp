
#include "UmlBaseComponentDiagram.h"
#include "UmlComponentDiagram.h"
#include "UmlComponentView.h"

UmlComponentDiagram * UmlBaseComponentDiagram::create(UmlComponentView * parent, const char * s)
{
  return (UmlComponentDiagram *) parent->create_(aComponentDiagram, s);
}

anItemKind UmlBaseComponentDiagram::kind() {
  return aComponentDiagram;
}

