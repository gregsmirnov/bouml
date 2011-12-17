
#include "UmlBaseExpansionNode.h"
#include "UmlExpansionNode.h"
#include "UmlExpansionRegion.h"

UmlExpansionNode * UmlBaseExpansionNode::create(UmlExpansionRegion * parent, const char * s)
{
  return (UmlExpansionNode *) parent->create_(anExpansionNode, s);
}

anItemKind UmlBaseExpansionNode::kind() {
  return anExpansionNode;
}

