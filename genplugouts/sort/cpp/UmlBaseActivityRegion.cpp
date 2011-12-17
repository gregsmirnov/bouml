
#include "UmlCom.h"
#include "UmlBaseActivityRegion.h"
#include "UmlActivityDiagram.h"

UmlActivityDiagram * UmlBaseActivityRegion::associatedDiagram() {
    read_if_needed_();
  
    return _assoc_diagram;
}

bool UmlBaseActivityRegion::set_AssociatedDiagram(UmlActivityDiagram * d) {
    UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);
    if (UmlCom::read_bool()) {
      _assoc_diagram = d;
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBaseActivityRegion::read_uml_() {
    _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();
    UmlBaseItem::read_uml_();
}

