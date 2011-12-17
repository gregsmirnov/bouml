#ifndef _UMLBASEINITIALPSEUDOSTATE_H
#define _UMLBASEINITIALPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlInitialPseudoState;
class UmlItem;

class UmlBaseInitialPseudoState : public UmlPseudoState {
  public:
    // returns a new initial pseudo state created under 'parent'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    
    static UmlInitialPseudoState * create(UmlItem * parent);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseInitialPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }

};

#endif
