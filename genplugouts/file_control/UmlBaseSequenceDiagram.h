#ifndef _UMLBASESEQUENCEDIAGRAM_H
#define _UMLBASESEQUENCEDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlSequenceDiagram;
class UmlItem;

// Manage the sequence diagrams
class UmlBaseSequenceDiagram : public UmlDiagram {
  public:
    // returns a new sequence diagram named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlSequenceDiagram * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseSequenceDiagram(void * id, const QCString & n) : UmlDiagram(id, n) {};

};

#endif
