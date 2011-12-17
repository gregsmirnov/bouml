
#include "UmlCom.h"
#include "UmlBaseActivityPartition.h"
#include "UmlActivityPartition.h"
#include "UmlActivityDiagram.h"

UmlActivityPartition * UmlBaseActivityPartition::create(UmlItem * parent, const char * s)
{
  return (UmlActivityPartition *) parent->create_(aPartition, s);
}

anItemKind UmlBaseActivityPartition::kind() {
  return aPartition;
}

UmlActivityDiagram * UmlBaseActivityPartition::associatedDiagram() {
  read_if_needed_();

  return _assoc_diagram;
}

bool UmlBaseActivityPartition::set_AssociatedDiagram(UmlActivityDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseActivityPartition::isDimension() {
  read_if_needed_();
  return _dimension;
}

bool UmlBaseActivityPartition::set_isDimension(bool v) {
  return set_it_(_dimension, v, setMultiplicityCmd);
}

bool UmlBaseActivityPartition::isExternal() {
  read_if_needed_();
  return _external;
}

bool UmlBaseActivityPartition::set_isExternal(bool v) {
  return set_it_(_external, v, setIsCppExternalCmd);
}

UmlItem * UmlBaseActivityPartition::represents() {
  read_if_needed_();
  return _represents;
}

bool UmlBaseActivityPartition::set_Represents(UmlItem * v) {
  UmlCom::send_cmd(_identifier, setDerivedCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
  if (UmlCom::read_bool()) {
    _represents = v;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseActivityPartition::read_uml_() {
  _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
  _dimension = UmlCom::read_bool();
  _external = UmlCom::read_bool();
  _represents = (UmlItem *) UmlBaseItem::read_();
}

