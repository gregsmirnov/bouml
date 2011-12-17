#ifndef _UMLBASEACTIVITYPARTITION_H
#define _UMLBASEACTIVITYPARTITION_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlActivityPartition;
class UmlActivityDiagram;

class UmlBaseActivityPartition : public UmlItem {
  public:
    //  returns a new activity partition named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlActivityPartition * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivityPartition(void * id, const QCString & s) : UmlItem(id, s) {
    }


  public:
    // returns the optional associated diagram
    UmlActivityDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlActivityDiagram * d);

    // return the  return the isDimension attribute
    bool isDimension();

    // set the isDimension attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isDimension(bool v);

    // return the  return the isExternal attribute
    bool isExternal();

    // set the isExternal attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isExternal(bool v);

    // return the represents
    UmlItem * represents();

    // set the represents
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Represents(UmlItem * v);


  private:
    UmlActivityDiagram * _assoc_diagram;

    bool _dimension;

    bool _external;

    UmlItem * _represents;


  protected:
    virtual void read_uml_();

};

#endif
