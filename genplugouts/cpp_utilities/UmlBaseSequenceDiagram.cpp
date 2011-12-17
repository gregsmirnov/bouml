
#include "UmlCom.h"
#include "UmlBaseSequenceDiagram.h"
#include "UmlSequenceDiagram.h"
#include "UmlItem.h"
#include "UmlSequenceDiagramDefinition.h"

UmlSequenceDiagram * UmlBaseSequenceDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlSequenceDiagram *) parent->create_(aSequenceDiagram, s);
}

anItemKind UmlBaseSequenceDiagram::kind() {
  return aSequenceDiagram;
}

UmlSequenceDiagramDefinition * UmlBaseSequenceDiagram::definition() {
  if (_def == 0) {
    UmlCom::send_cmd(_identifier, sideCmd);
    (_def = new UmlSequenceDiagramDefinition())->read_();
  }
  return _def;
}

void UmlBaseSequenceDiagram::unload(bool rec, bool del) {
  if (_def != 0) { delete _def; _def = 0; }
  UmlBaseDiagram::unload(rec, del);
}

