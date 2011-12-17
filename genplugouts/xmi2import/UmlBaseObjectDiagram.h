#ifndef _UMLBASEOBJECTDIAGRAM_H
#define _UMLBASEOBJECTDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlObjectDiagram;
class UmlItem;

class UmlBaseObjectDiagram : public UmlDiagram {
  public:
    //  returns a new object diagram named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlObjectDiagram * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseObjectDiagram(void * id, const QCString & s) : UmlDiagram(id, s) {
    }

};

#endif
