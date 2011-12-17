#ifndef _UMLBASEJUNCTIONPSEUDOSTATE_H
#define _UMLBASEJUNCTIONPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlJunctionPseudoState;
class UmlItem;

class UmlBaseJunctionPseudoState : public UmlPseudoState {
  public:
    // returns a new junction pseudo state created under 'parent'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    
    static UmlJunctionPseudoState * create(UmlItem * parent);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseJunctionPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }

};

#endif
