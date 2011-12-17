
#include "UmlCom.h"
#include "UmlBaseClassInstance.h"
#include "UmlClassInstance.h"
#include "UmlClass.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlObjectDiagram.h"

anItemKind UmlBaseClassInstance::kind() {
  return aClassInstance;
}

UmlClassInstance * UmlBaseClassInstance::create(UmlItem * parent, const char * name, UmlClass * type)
{
  UmlCom::send_cmd(parent->_identifier, createCmd, aClassInstance,
		   type->_identifier);
  UmlClassInstance * result = (UmlClassInstance *) UmlBaseItem::read_();

  if (result != 0) {
    parent->reread_children_if_needed_();
    if (name != 0) result->set_Name(name);
  }
  return result;
}

UmlClass * UmlBaseClassInstance::type() {
  read_if_needed_();
  return _type;
}

bool UmlBaseClassInstance::set_Type(UmlClass * v) {
  UmlCom::send_cmd(_identifier, setTypeCmd, ((UmlBaseItem *) v)->_identifier);
  if (UmlCom::read_bool()) {
    _type = v;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseClassInstance::attributesValue(QValueList<SlotAttribute> & result) {
  UmlCom::send_cmd(_identifier, attributesCmd, (char) 0);

  unsigned n = UmlCom::read_unsigned();

  result.clear();
  while (n--) {
    UmlAttribute * at = (UmlAttribute *) UmlBaseItem::read_();

    result.append(SlotAttribute(at, UmlCom::read_string()));
  }
}

void UmlBaseClassInstance::relationsValue(QValueList<SlotRelation> & result) {
  UmlCom::send_cmd(_identifier, relationsCmd, (void *) 0);

  unsigned n = UmlCom::read_unsigned();

  result.clear();
  while (n--) {
    UmlRelation * r = (UmlRelation *) UmlBaseItem::read_();

    result.append(SlotRelation(r, (UmlClassInstance *) UmlBaseItem::read_()));
  }
}

void UmlBaseClassInstance::availableAttributes(QVector<UmlAttribute> & result) {
  UmlCom::send_cmd(_identifier, attributesCmd, (char) 1);
  UmlCom::read_item_list((QVector<UmlItem> &) result);
}

void UmlBaseClassInstance::availableRelations(UmlClassInstance * other, QVector<UmlRelation> & result) {
  UmlCom::send_cmd(_identifier, relationsCmd, other->_identifier);
  UmlCom::read_item_list((QVector<UmlItem> &) result);
}

bool UmlBaseClassInstance::set_AttributeValue(UmlAttribute * attribute, const char * value) {
  UmlCom::send_cmd(_identifier, setAttributeCmd, attribute->_identifier, value);
  return UmlCom::read_bool();
}

bool UmlBaseClassInstance::add_Relation(UmlRelation * relation, UmlClassInstance * other) {
  QVector<UmlItem> v(2);

  v.insert(0, relation);
  v.insert(1, other);
  UmlCom::send_cmd(_identifier, addRelationCmd, v);
  return UmlCom::read_bool();
}

bool UmlBaseClassInstance::remove_Relation(UmlRelation * relation, UmlClassInstance * other) {
  QVector<UmlItem> v(2);

  v.insert(0, relation);
  v.insert(1, other);
  UmlCom::send_cmd(_identifier, removeRelationCmd, v);
  return UmlCom::read_bool();
}

UmlObjectDiagram * UmlBaseClassInstance::associatedDiagram() {
  read_if_needed_();

  return _assoc_diagram;
}

bool UmlBaseClassInstance::set_AssociatedDiagram(UmlObjectDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseClassInstance::read_uml_() {
  _assoc_diagram = (UmlObjectDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
  _type = (UmlClass *) UmlBaseItem::read_();
}

