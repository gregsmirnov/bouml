#ifndef _UMLDEEPHISTORYPSEUDOSTATE_H
#define _UMLDEEPHISTORYPSEUDOSTATE_H


#include <qcstring.h>
#include "UmlBaseDeepHistoryPseudoState.h"

class UmlDeepHistoryPseudoState : public UmlBaseDeepHistoryPseudoState {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlDeepHistoryPseudoState(void * id) : UmlBaseDeepHistoryPseudoState(id, "") {
    }

};

#endif
