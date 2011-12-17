
#include "UmlBaseClassDiagram.h"
#include "UmlClassDiagram.h"
#include "UmlItem.h"

UmlClassDiagram * UmlBaseClassDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlClassDiagram *) parent->create_(aClassDiagram, s);
}

anItemKind UmlBaseClassDiagram::kind() {
  return aClassDiagram;
}

