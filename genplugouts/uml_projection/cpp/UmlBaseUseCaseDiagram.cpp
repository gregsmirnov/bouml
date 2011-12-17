
#include "UmlCom.h"
#include "UmlBaseUseCaseDiagram.h"
#include "UmlUseCaseDiagram.h"
#include "UmlItem.h"
#include "UmlUseCaseDiagramDefinition.h"

UmlUseCaseDiagram * UmlBaseUseCaseDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlUseCaseDiagram *) parent->create_(anUseCaseDiagram, s);
}

anItemKind UmlBaseUseCaseDiagram::kind() {
  return anUseCaseDiagram;
}

UmlUseCaseDiagramDefinition * UmlBaseUseCaseDiagram::definition() {
  if (_def == 0) {
    UmlCom::send_cmd(_identifier, sideCmd);
    (_def = new UmlUseCaseDiagramDefinition())->read_();
  }
  return _def;
}

void UmlBaseUseCaseDiagram::unload(bool rec, bool del) {
  if (_def != 0) { delete _def; _def = 0; }
  UmlBaseDiagram::unload(rec, del);
}

