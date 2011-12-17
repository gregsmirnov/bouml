
#include "UmlBaseNode.h"
#include "UmlNode.h"
#include "UmlDeploymentDiagram.h"

#include "UmlCom.h"
UmlNode * UmlBaseNode::create(UmlItem * parent, const char * s)
{
  return (UmlNode *) parent->create_(aNode, s);
}

anItemKind UmlBaseNode::kind() {
  return aNode;
}

UmlDeploymentDiagram * UmlBaseNode::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBaseNode::set_AssociatedDiagram(UmlDeploymentDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseNode::read_uml_() {
  _assoc_diagram = (UmlDeploymentDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
}

