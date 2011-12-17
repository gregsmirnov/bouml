#ifndef UMLCOMPONENT_H
#define UMLCOMPONENT_H

#include "UmlBaseComponent.h"

// This class manages 'components'
// You can modify it as you want (except the constructor)

class UmlComponent : public UmlBaseComponent {
  public:
    UmlComponent(void * id, const QCString & n)
      : UmlBaseComponent(id, n) {};
};

#endif
