#ifndef _UMLFINALSTATE_H
#define _UMLFINALSTATE_H


#include "UmlBaseFinalState.h"

class UmlFinalState : public UmlBaseFinalState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlFinalState(void * id) : UmlBaseFinalState(id, "") {
    }

};

#endif
