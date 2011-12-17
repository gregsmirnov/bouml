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

    // return the the referenced sub machine state or 0/null
    // if the state is not a sub machine state reference
    UmlExitPointPseudoState * reference();

    // set the referenced sub machine state (may be 0/null)
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Reference(UmlExitPointPseudoState * v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseExitPointPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }


  private:
    UmlExitPointPseudoState * _reference;


  protected:
    virtual void read_uml_();

};

#endif
