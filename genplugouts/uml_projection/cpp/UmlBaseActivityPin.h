#ifndef _UMLBASEACTIVITYPIN_H
#define _UMLBASEACTIVITYPIN_H


#include "UmlPinParameter.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlActivityPin;
class UmlActivityAction;

class UmlBaseActivityPin : public UmlPinParameter {
  public:
    //  returns a new activity action pin named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlActivityPin * create(UmlActivityAction * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivityPin(void * id, const QCString & s) : UmlPinParameter(id, s) {
    }

};

#endif
