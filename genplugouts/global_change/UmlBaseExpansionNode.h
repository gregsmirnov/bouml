#ifndef _UMLBASEEXPANSIONNODE_H
#define _UMLBASEEXPANSIONNODE_H


#include "UmlActivityObject.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlExpansionNode;
class UmlExpansionRegion;

class UmlBaseExpansionNode : public UmlActivityObject {
  public:
    //  returns a new expansion node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlExpansionNode * create(UmlExpansionRegion * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseExpansionNode(void * id, const QCString & s) : UmlActivityObject(id, s) {
    }

};

#endif
