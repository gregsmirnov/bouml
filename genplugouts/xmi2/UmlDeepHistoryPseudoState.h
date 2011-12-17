#ifndef _UMLDEEPHISTORYPSEUDOSTATE_H
#define _UMLDEEPHISTORYPSEUDOSTATE_H


#include "UmlBaseDeepHistoryPseudoState.h"

class UmlDeepHistoryPseudoState : public UmlBaseDeepHistoryPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlDeepHistoryPseudoState(void * id) : UmlBaseDeepHistoryPseudoState(id, "") {
    }

    virtual const char * sKind() const;

};

#endif
