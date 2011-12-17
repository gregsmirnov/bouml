#ifndef _UMLBASEDEEPHISTORYPSEUDOSTATE_H
#define _UMLBASEDEEPHISTORYPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlDeepHistoryPseudoState;
class UmlItem;

class UmlBaseDeepHistoryPseudoState : public UmlPseudoState {
  public:
    // returns a new deep history pseudo state created under 'parent'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    
    static UmlDeepHistoryPseudoState * create(UmlItem * parent);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseDeepHistoryPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }

};

#endif
