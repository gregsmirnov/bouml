
#include "UmlBaseDeploymentView.h"
#include "UmlDeploymentView.h"
#include "UmlPackage.h"

UmlDeploymentView * UmlBaseDeploymentView::create(UmlPackage * parent, const char * s)
{
  return (UmlDeploymentView *) parent->create_(aDeploymentView, s);
}

anItemKind UmlBaseDeploymentView::kind() {
  return aDeploymentView;
}

