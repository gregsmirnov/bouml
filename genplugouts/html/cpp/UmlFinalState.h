#ifndef _UMLFINALSTATE_H
#define _UMLFINALSTATE_H


#include <qcstring.h>
#include "UmlBaseFinalState.h"

class UmlFinalState : public UmlBaseFinalState {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlFinalState(void * id) : UmlBaseFinalState(id, "") {
    }

};

#endif
