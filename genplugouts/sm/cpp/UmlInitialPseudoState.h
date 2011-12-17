#ifndef _UMLINITIALPSEUDOSTATE_H
#define _UMLINITIALPSEUDOSTATE_H


#include "UmlBaseInitialPseudoState.h"

class UmlClass;
class UmlState;

class UmlInitialPseudoState : public UmlBaseInitialPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlInitialPseudoState(void * id) : UmlBaseInitialPseudoState(id, "") {
    }

    virtual void generate(UmlClass * machine, UmlClass * anystate, UmlState * state);

};

#endif
