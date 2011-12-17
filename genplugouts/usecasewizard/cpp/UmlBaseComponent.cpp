
#include "UmlBaseComponent.h"
#include "UmlComponent.h"
#include "UmlComponentView.h"
#include "UmlComponentDiagram.h"
#include "UmlClass.h"

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

const QVector<UmlClass> & UmlBaseComponent::realizedClasses() {
  read_if_needed_();
  
  return _realized;
}

const QVector<UmlClass> & UmlBaseComponent::providedClasses() {
  read_if_needed_();
  
  return _provided;
}

const QVector<UmlClass> & UmlBaseComponent::requiredClasses() {
  read_if_needed_();
  
  return _required;
}

bool UmlBaseComponent::set_AssociatedClasses(const QVector<UmlClass> & realized, const QVector<UmlClass> & provided, const QVector<UmlClass> & required) {
  UmlCom::send_cmd(_identifier, setAssocClassesCmd,
		   realized, provided, required);
  if (UmlCom::read_bool()) {
    if (_defined) {
      _realized = realized;
      _provided = provided;
      _required = required;
    }
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseComponent::unload(bool rec, bool del) {
  _realized.clear();
  _provided.clear();
  _required.clear();

  UmlBaseItem::unload(rec, del);

}

void UmlBaseComponent::read_uml_() {
  _assoc_diagram = (UmlComponentDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  _realized.resize(n);
    
  for (index = 0; index != n; index += 1)
    _realized.insert(index, (UmlClass *) UmlBaseItem::read_());

  n = UmlCom::read_unsigned();
  _provided.resize(n);
    
  for (index = 0; index != n; index += 1)
    _provided.insert(index, (UmlClass *) UmlBaseItem::read_());

  n = UmlCom::read_unsigned();
  _required.resize(n);
    
  for (index = 0; index != n; index += 1)
    _required.insert(index, (UmlClass *) UmlBaseItem::read_());
}

