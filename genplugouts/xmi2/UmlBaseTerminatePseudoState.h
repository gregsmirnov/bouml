#ifndef _UMLBASETERMINATEPSEUDOSTATE_H
#define _UMLBASETERMINATEPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlTerminatePseudoState;
class UmlItem;

class UmlBaseTerminatePseudoState : public UmlPseudoState {
  public:
    // returns a new terminate pseudo state created under 'parent'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    
    static UmlTerminatePseudoState * create(UmlItem * parent);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseTerminatePseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }

};

#endif
