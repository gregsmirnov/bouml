#ifndef _UMLBASEREGION_H
#define _UMLBASEREGION_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlRegion;
class UmlState;

class UmlBaseRegion : public UmlItem {
  public:
    //  returns a new region named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRegion * create(UmlState * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseRegion(void * id, const QCString & s) : UmlItem(id, s) {
    }

};

#endif
