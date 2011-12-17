#ifndef _UMLBASECOMPONENT_H
#define _UMLBASECOMPONENT_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>

#include "UmlClass.h"	// to avoid destructor problem
class UmlComponent;
class UmlComponentView;
class UmlComponentDiagram;

//  Manage the components.
class UmlBaseComponent : public UmlItem {
  public:
    // returns a new component named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlComponent * create(UmlComponentView * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the optional associated diagram
    UmlComponentDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlComponentDiagram * d);


  private:
    UmlComponentDiagram * _assoc_diagram;


  protected:
    virtual void read_uml_();

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseComponent(void * id, const QCString & n);

};

inline UmlBaseComponent::UmlBaseComponent(void * id, const QCString & n) : UmlItem(id, n) {
  _assoc_diagram = 0;
}

#endif
