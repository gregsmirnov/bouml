#ifndef _UMLTRANSITION_H
#define _UMLTRANSITION_H


#include "UmlBaseTransition.h"
#include "UmlStateItem.h"

class UmlTransition : public UmlBaseTransition, public UmlStateItem {
  public:
     UmlTransition(void * id) : UmlBaseTransition(id) {
    }

};

#endif
