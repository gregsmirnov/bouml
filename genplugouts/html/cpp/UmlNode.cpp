
#include "UmlNode.h"

#include "UmlCom.h"
QCString UmlNode::sKind() {
  return "node";
}

void UmlNode::html(QCString, unsigned int, unsigned int) {
  UmlCom::message(name());

  UmlItem::html("Node", (UmlDiagram *) associatedDiagram());

  unload(FALSE, FALSE);
}

