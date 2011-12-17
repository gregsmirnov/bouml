#ifndef _UMLBASEJOINPSEUDOSTATE_H
#define _UMLBASEJOINPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlJoinPseudoState;
class UmlItem;

class UmlBaseJoinPseudoState : public UmlPseudoState {
  public:
    // returns a new join pseudo state created under 'parent'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    
    static UmlJoinPseudoState * create(UmlItem * parent);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseJoinPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }

};

#endif
