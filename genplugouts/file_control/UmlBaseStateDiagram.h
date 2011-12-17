#ifndef _UMLBASESTATEDIAGRAM_H
#define _UMLBASESTATEDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlStateDiagram;
class UmlState;

class UmlBaseStateDiagram : public UmlDiagram {
  public:
    //  returns a new state diagram named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlStateDiagram * create(UmlState * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseStateDiagram(void * id, const QCString & s) : UmlDiagram(id, s) {
    }

};

#endif
