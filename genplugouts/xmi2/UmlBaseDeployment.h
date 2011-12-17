#ifndef _UMLBASEDEPLOYMENT_H
#define _UMLBASEDEPLOYMENT_H


#include "UmlItem.h"
#include <qcstring.h>

//  Manage the deployments
class UmlBaseDeployment : public UmlItem {
  protected:
    UmlBaseDeployment(void * id, QCString n) : UmlItem(id, n){};

};

#endif
