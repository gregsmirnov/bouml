#ifndef _UMLSTATEACTION_H
#define _UMLSTATEACTION_H


#include "UmlBaseStateAction.h"
#include "UmlStateItem.h"
#include <qcstring.h>

class UmlClass;
class UmlState;

class UmlStateAction : public UmlBaseStateAction, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlStateAction(void * id) : UmlBaseStateAction(id, "") {
    }

    //  is the target of a transition
    virtual void generate(UmlClass * machine, UmlClass * anystate, UmlState * state, QCString & body, QCString indent);

};

#endif
