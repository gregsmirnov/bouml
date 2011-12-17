#ifndef _UMLBASECOMPONENTDIAGRAM_H
#define _UMLBASECOMPONENTDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlComponentDiagram;
class UmlComponentView;

//  Manage the component diagrams
class UmlBaseComponentDiagram : public UmlDiagram {
  public:
    // returns a new component diagram named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlComponentDiagram * create(UmlComponentView * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseComponentDiagram(void * id, const QCString & n) : UmlDiagram(id, n) {};

};

#endif
