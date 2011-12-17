#ifndef _UMLBASEEXITPOINTPSEUDOSTATE_H
#define _UMLBASEEXITPOINTPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlExitPointPseudoState;
class UmlItem;

class UmlBaseExitPointPseudoState : public UmlPseudoState {
  public:
    //  returns a new exit point pseudo state named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlExitPointPseudoState * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseExitPointPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {}
    virtual void read_uml_();
};

#endif
