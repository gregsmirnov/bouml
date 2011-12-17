#ifndef _UMLSTATEACTION_H
#define _UMLSTATEACTION_H


#include "UmlBaseStateAction.h"
#include "UmlStateItem.h"

class UmlStateAction : public UmlBaseStateAction, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlStateAction(void * id) : UmlBaseStateAction(id, "") {
    }

};

#endif
