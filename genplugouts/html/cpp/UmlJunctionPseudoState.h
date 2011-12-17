#ifndef _UMLJUNCTIONPSEUDOSTATE_H
#define _UMLJUNCTIONPSEUDOSTATE_H


#include <qcstring.h>
#include "UmlBaseJunctionPseudoState.h"

class UmlJunctionPseudoState : public UmlBaseJunctionPseudoState {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlJunctionPseudoState(void * id) : UmlBaseJunctionPseudoState(id, "") {
    }

};

#endif
