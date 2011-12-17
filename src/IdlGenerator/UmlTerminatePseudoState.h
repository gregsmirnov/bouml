#ifndef _UMLTERMINATEPSEUDOSTATE_H
#define _UMLTERMINATEPSEUDOSTATE_H


#include "UmlBaseTerminatePseudoState.h"

class UmlTerminatePseudoState : public UmlBaseTerminatePseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlTerminatePseudoState(void * id) : UmlBaseTerminatePseudoState(id, "") {
    }

};

#endif
