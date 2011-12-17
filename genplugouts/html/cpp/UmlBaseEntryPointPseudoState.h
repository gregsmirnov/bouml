#ifndef _UMLBASEENTRYPOINTPSEUDOSTATE_H
#define _UMLBASEENTRYPOINTPSEUDOSTATE_H


#include "UmlPseudoState.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlEntryPointPseudoState;
class UmlItem;

class UmlBaseEntryPointPseudoState : public UmlPseudoState {
  public:
    //  returns a new entry point pseudo state named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlEntryPointPseudoState * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the the referenced sub machine state or 0/null
    // if the state is not a sub machine state reference
    UmlEntryPointPseudoState * reference();

    // set the referenced sub machine state (may be 0/null)
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Reference(UmlEntryPointPseudoState * v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseEntryPointPseudoState(void * id, const QCString & s) : UmlPseudoState(id, s) {
    }


  private:
    UmlEntryPointPseudoState * _reference;


  protected:
    virtual void read_uml_();

};

#endif
