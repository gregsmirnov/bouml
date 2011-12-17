
#include "UmlCom.h"
#include "UmlBaseCollaborationDiagram.h"
#include "UmlCollaborationDiagram.h"
#include "UmlItem.h"
#include "UmlCollaborationDiagramDefinition.h"

UmlCollaborationDiagram * UmlBaseCollaborationDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlCollaborationDiagram *) parent->create_(aCollaborationDiagram, s);
}

anItemKind UmlBaseCollaborationDiagram::kind() {
  return aCollaborationDiagram;
}

UmlCollaborationDiagramDefinition * UmlBaseCollaborationDiagram::definition() {
  if (_def == 0) {
    UmlCom::send_cmd(_identifier, sideCmd);
    (_def = new UmlCollaborationDiagramDefinition())->read_();
  }
  return _def;
}

void UmlBaseCollaborationDiagram::unload(bool rec, bool del) {
  if (_def != 0) { delete _def; _def = 0; }
  UmlBaseDiagram::unload(rec, del);
}

