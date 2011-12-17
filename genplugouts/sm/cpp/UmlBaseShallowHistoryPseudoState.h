#ifndef _UMLBASESHALLOWHISTORYPSEUDOSTATE_H
#define _UMLBASESHALLOWHISTORYPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlShallowHistoryPseudoState;
class UmlItem;

class UmlBaseShallowHistoryPseudoState : public UmlPseudoState {
  public:
    // returns a new shallow history pseudo state created under 'parent'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    
    static UmlShallowHistoryPseudoState * create(UmlItem * parent);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseShallowHistoryPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }

};

#endif
