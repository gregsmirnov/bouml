#ifndef _UMLFORKPSEUDOSTATE_H
#define _UMLFORKPSEUDOSTATE_H


#include "UmlBaseForkPseudoState.h"

class UmlForkPseudoState : public UmlBaseForkPseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlForkPseudoState(void * id) : UmlBaseForkPseudoState(id, "") {
    }

    virtual const char * sKind() const;

};

#endif
