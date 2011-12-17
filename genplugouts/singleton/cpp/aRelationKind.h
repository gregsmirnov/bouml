#ifndef _ARELATIONKIND_H
#define _ARELATIONKIND_H


// The king of a relation, returned by UmlBaseRelation::RelationKind()
enum aRelationKind {
  anAssociation,
  aDirectionalAssociation,
  aGeneralisation,
  anAggregation,
  anAggregationByValue,
  aDirectionalAggregation,
  aDirectionalAggregationByValue,
  aDependency,
  aRealization

};
#endif
