#ifndef _UMLCOMPONENT_H
#define _UMLCOMPONENT_H


#include "UmlBaseComponent.h"
#include <qcstring.h>

// This class manages 'components'
//
// You can modify it as you want (except the constructor)
class UmlComponent : public UmlBaseComponent {
  public:
    UmlComponent(void * id, const QCString & n) : UmlBaseComponent(id, n) {};

    virtual int orderWeight();

};

#endif
