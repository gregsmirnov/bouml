#ifndef _UMLBASECOMPONENT_H
#define _UMLBASECOMPONENT_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qvector.h>
#include <qcstring.h>

#include "UmlClass.h"	// to avoid destructor problem
class UmlComponent;
class UmlComponentView;
class UmlComponentDiagram;
class UmlClass;

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

    // returns (in Java a copy of) the optional realizing classes
    const QVector<UmlClass> & realizingClasses();

    // returns (in Java a copy of) the optional provided classes
    const QVector<UmlClass> & providedClasses();

    // returns (in Java a copy of) the optional required classes
    const QVector<UmlClass> & requiredClasses();


  private:
    UmlComponentDiagram * _assoc_diagram;

    QVector<UmlClass> _realizing;

    QVector<UmlClass> _provided;

    QVector<UmlClass> _required;


  protected:
    virtual void read_uml_();

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseComponent(void * id, const QCString & n);

};

inline UmlBaseComponent::UmlBaseComponent(void * id, const QCString & n) : UmlItem(id, n) {
  _assoc_diagram = 0;
}

#endif
