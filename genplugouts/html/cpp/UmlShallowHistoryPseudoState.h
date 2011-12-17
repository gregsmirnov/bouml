#ifndef _UMLSHALLOWHISTORYPSEUDOSTATE_H
#define _UMLSHALLOWHISTORYPSEUDOSTATE_H


#include <qcstring.h>
#include "UmlBaseShallowHistoryPseudoState.h"

class UmlShallowHistoryPseudoState : public UmlBaseShallowHistoryPseudoState {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlShallowHistoryPseudoState(void * id) : UmlBaseShallowHistoryPseudoState(id, "") {
    }

};

#endif
