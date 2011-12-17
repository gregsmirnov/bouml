
#include "UmlBaseUseCase.h"
#include "UmlUseCase.h"
#include "UmlUseCaseDiagram.h"

#include "UmlCom.h"
UmlUseCase * UmlBaseUseCase::create(UmlItem * parent, const char * s)
{
  return (UmlUseCase *) parent->create_(anUseCase, s);
}

anItemKind UmlBaseUseCase::kind() {
  return anUseCase;
}

UmlUseCaseDiagram * UmlBaseUseCase::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBaseUseCase::set_AssociatedDiagram(UmlUseCaseDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseUseCase::read_uml_() {
  _assoc_diagram = (UmlUseCaseDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
}

