
#include "UmlBaseComponent.h"
#include "UmlComponent.h"
#include "UmlComponentView.h"
#include "UmlComponentDiagram.h"

#include "UmlCom.h"
UmlComponent * UmlBaseComponent::create(UmlComponentView * parent, const char * s)
{
  return (UmlComponent *) parent->create_(aComponent, s);
}

anItemKind UmlBaseComponent::kind() {
  return aComponent;
}

UmlComponentDiagram * UmlBaseComponent::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBaseComponent::set_AssociatedDiagram(UmlComponentDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseComponent::read_uml_() {
  _assoc_diagram = (UmlComponentDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
}

