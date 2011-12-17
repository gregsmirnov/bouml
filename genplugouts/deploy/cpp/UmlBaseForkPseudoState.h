#ifndef _UMLBASEFORKPSEUDOSTATE_H
#define _UMLBASEFORKPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlForkPseudoState;
class UmlItem;

class UmlBaseForkPseudoState : public UmlPseudoState {
  public:
    // returns a new fork pseudo state created under 'parent'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    
    static UmlForkPseudoState * create(UmlItem * parent);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseForkPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }

};

#endif
