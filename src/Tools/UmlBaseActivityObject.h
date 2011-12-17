#ifndef _UMLBASEACTIVITYOBJECT_H
#define _UMLBASEACTIVITYOBJECT_H


#include "UmlActivityNode.h"
#include "anItemKind.h"
#include <qcstring.h>
#include "UmlTypeSpec.h"
#include "anOrdering.h"

class UmlActivityObject;
class UmlItem;
class UmlActivityDiagram;

class UmlBaseActivityObject : public UmlActivityNode {
  public:
    //  returns a new activity object named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlActivityObject * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivityObject(void * id, const QCString & s) : UmlActivityNode(id, s) {
    }


  public:
    // return the type
    const UmlTypeSpec & type();

    // set the type
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Type(const UmlTypeSpec & v);

    // return the multiplicity
    const QCString & multiplicity();

    // set the multiplicity
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Multiplicity(const char * v);

    // return the inState attribute
    const QCString & inState();

    // set the inState attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_InState(const char * v);

    // return the selection in OCL
    const QCString & selection();

    // set the selection in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Selection(const char * v);

#ifdef WITHCPP
    // return the selection in C++
    const QCString & cppSelection();

    // set the selection in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppSelection(const char * v);
#endif

#ifdef WITHJAVA
    // return the selection in Java
    const QCString & javaSelection();

    // set the selection in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaSelection(const char * v);
#endif

    // return the ordering
    anOrdering ordering();

    // set the ordering
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Ordering(anOrdering v);

    // return the isControlType attribute, tells whether the type of the object node is to be treated as control
    bool isControlType();

    // set the isControlType attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IsControlType(bool v);

    // returns the optional associated diagram
    UmlActivityDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlActivityDiagram * d);

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    UmlActivityDiagram * _assoc_diagram;

    UmlTypeSpec _type;

    QCString _multiplicity;

    QCString _in_state;

    QCString _selection;

#ifdef WITHCPP
    QCString _cpp_selection;
#endif

#ifdef WITHJAVA
    QCString _java_selection;
#endif

    anOrdering _ordering : 8;

    bool _is_control : 1;


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

#endif
