#ifndef _UMLCHOICEPSEUDOSTATE_H
#define _UMLCHOICEPSEUDOSTATE_H


#include "UmlBaseChoicePseudoState.h"

class UmlChoicePseudoState : public UmlBaseChoicePseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlChoicePseudoState(void * id) : UmlBaseChoicePseudoState(id, "") {
    }

};

#endif
