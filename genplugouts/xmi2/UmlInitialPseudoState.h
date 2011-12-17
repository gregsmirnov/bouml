#ifndef _UMLINITIALPSEUDOSTATE_H
#define _UMLINITIALPSEUDOSTATE_H


#include "UmlBaseInitialPseudoState.h"

class UmlInitialPseudoState : public UmlBaseInitialPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlInitialPseudoState(void * id) : UmlBaseInitialPseudoState(id, "") {
    }

    virtual const char * sKind() const;

};

#endif
