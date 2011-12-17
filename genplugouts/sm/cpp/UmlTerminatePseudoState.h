#ifndef _UMLTERMINATEPSEUDOSTATE_H
#define _UMLTERMINATEPSEUDOSTATE_H


#include "UmlBaseTerminatePseudoState.h"
#include <qcstring.h>

class UmlClass;
class UmlState;

class UmlTerminatePseudoState : public UmlBaseTerminatePseudoState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlTerminatePseudoState(void * id) : UmlBaseTerminatePseudoState(id, "") {
    }

    virtual void generate(UmlClass * machine, UmlClass * anystate, UmlState * state, QCString & body, QCString indent);

};

#endif
