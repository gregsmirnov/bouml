
#include "UmlBaseDeploymentDiagram.h"
#include "UmlDeploymentDiagram.h"
#include "UmlDeploymentView.h"

UmlDeploymentDiagram * UmlBaseDeploymentDiagram::create(UmlDeploymentView * parent, const char * s)
{
  return (UmlDeploymentDiagram *) parent->create_(aDeploymentDiagram, s);
}

anItemKind UmlBaseDeploymentDiagram::kind() {
  return aDeploymentDiagram;
}

