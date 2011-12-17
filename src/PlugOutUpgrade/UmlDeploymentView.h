#ifndef UMLDEPLOYMENTVIEW_H
#define UMLDEPLOYMENTVIEW_H

#include "UmlBaseDeploymentView.h"

// This class manages 'deployment view'
// You can modify it as you want (except the constructor)

class UmlDeploymentView : public UmlBaseDeploymentView {
  public:
    UmlDeploymentView(void * id, const QCString & n)
      : UmlBaseDeploymentView(id, n) {};
};

#endif
