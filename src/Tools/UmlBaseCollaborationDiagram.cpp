
#include "UmlBaseCollaborationDiagram.h"
#include "UmlCollaborationDiagram.h"
#include "UmlItem.h"

UmlCollaborationDiagram * UmlBaseCollaborationDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlCollaborationDiagram *) parent->create_(aCollaborationDiagram, s);
}

anItemKind UmlBaseCollaborationDiagram::kind() {
  return aCollaborationDiagram;
}

