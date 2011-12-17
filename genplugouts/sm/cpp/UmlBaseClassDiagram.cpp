
#include "UmlBaseClassDiagram.h"
#include "UmlClassDiagram.h"
#include "UmlClassView.h"

UmlClassDiagram * UmlBaseClassDiagram::create(UmlClassView * parent, const char * s)
{
  return (UmlClassDiagram *) parent->create_(aClassDiagram, s);
}

anItemKind UmlBaseClassDiagram::kind() {
  return aClassDiagram;
}

