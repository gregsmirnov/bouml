#ifndef _UMLBASECOLLABORATIONDIAGRAM_H
#define _UMLBASECOLLABORATIONDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlCollaborationDiagram;
class UmlItem;

//  Manage the collaboration diagrams
class UmlBaseCollaborationDiagram : public UmlDiagram {
  public:
    // returns a new collaboration diagram named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCollaborationDiagram * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseCollaborationDiagram(void * id, const QCString & n) : UmlDiagram(id, n) {};

};

#endif
