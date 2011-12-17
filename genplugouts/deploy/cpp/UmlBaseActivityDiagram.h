#ifndef _UMLBASEACTIVITYDIAGRAM_H
#define _UMLBASEACTIVITYDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlActivityDiagram;
class UmlItem;

class UmlBaseActivityDiagram : public UmlDiagram {
  public:
    //  returns a new activity diagram named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlActivityDiagram * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivityDiagram(void * id, const QCString & s) : UmlDiagram(id, s) {
    }

};

#endif
