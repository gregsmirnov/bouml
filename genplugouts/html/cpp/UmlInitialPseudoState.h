#ifndef _UMLINITIALPSEUDOSTATE_H
#define _UMLINITIALPSEUDOSTATE_H


#include <qcstring.h>
#include "UmlBaseInitialPseudoState.h"

class UmlInitialPseudoState : public UmlBaseInitialPseudoState {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlInitialPseudoState(void * id) : UmlBaseInitialPseudoState(id, "") {
    }

};

#endif
