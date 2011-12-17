#ifndef _UMLBASEDEPLOYMENTDIAGRAM_H
#define _UMLBASEDEPLOYMENTDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlDeploymentDiagram;
class UmlDeploymentView;

//  Manage the deployment diagrams
class UmlBaseDeploymentDiagram : public UmlDiagram {
  public:
    // returns a new deployment diagram named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlDeploymentDiagram * create(UmlDeploymentView * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseDeploymentDiagram(void * id, const QCString & n) : UmlDiagram(id, n) {};

};

#endif
