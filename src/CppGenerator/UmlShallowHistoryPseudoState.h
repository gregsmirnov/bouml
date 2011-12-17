#ifndef _UMLSHALLOWHISTORYPSEUDOSTATE_H
#define _UMLSHALLOWHISTORYPSEUDOSTATE_H


#include "UmlBaseShallowHistoryPseudoState.h"

class UmlShallowHistoryPseudoState : public UmlBaseShallowHistoryPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlShallowHistoryPseudoState(void * id) : UmlBaseShallowHistoryPseudoState(id, "") {
    }

};

#endif
