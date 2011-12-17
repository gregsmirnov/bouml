#ifndef _UMLBASENODE_H
#define _UMLBASENODE_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlNode;
class UmlDeploymentDiagram;

//  Manage the nodes
class UmlBaseNode : public UmlItem {
  public:
    // returns a new node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlNode * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the optional associated diagram
    UmlDeploymentDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlDeploymentDiagram * d);


  private:
    UmlDeploymentDiagram * _assoc_diagram;


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseNode(void * id, const QCString & n) : UmlItem(id, n) {};

    //internal, do NOT use it
    
    virtual void read_uml_();

};

#endif
