
#include "UmlBaseNcRelation.h"
#include "UmlNcRelation.h"

#include "UmlCom.h"
UmlNcRelation * UmlBaseNcRelation::create(aRelationKind kind, UmlItem * start, UmlItem * end)
{
  UmlCom::send_cmd(start->_identifier, createCmd, aNcRelation,
		   kind, end->_identifier);
  UmlNcRelation * result = (UmlNcRelation *) UmlBaseItem::read_();
  
  if (result != 0)
    start->reread_children_if_needed_();
  
  return result;
}

anItemKind UmlBaseNcRelation::kind() {
  return aNcRelation;
}

aRelationKind UmlBaseNcRelation::relationKind() {
  read_if_needed_();
  
  return _rel_kind;
}

UmlItem * UmlBaseNcRelation::target() {
  read_if_needed_();
  
  return _target;
}

void UmlBaseNcRelation::read_uml_() {
  UmlBaseItem::read_uml_();
  _rel_kind = (aRelationKind) UmlCom::read_char();
  _target = UmlBaseItem::read_();
}

