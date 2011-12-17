#ifndef _UMLBASESTATE_H
#define _UMLBASESTATE_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>
#include "StateBehavior.h"

class UmlState;
class UmlOperation;
class UmlStateDiagram;

class UmlBaseState : public UmlItem {
  public:
    //  returns a new state named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlState * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseState(void * id, const QCString & s) : UmlItem(id, s) {
    }


  public:
    // return the entry behavior in OCL
    const QCString & entryBehavior();

    // set the entry behavior in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_EntryBehavior(const char * s);

    // return the exit behavior in OCL
    const QCString & exitBehavior();

    // set the exit behavior in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_ExitBehavior(const char * s);

    // return the activity in OCL
    const QCString & doActivity();

    // set the activity in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_DoActivity(const char * s);

#ifdef WITHCPP
    // return the entry behavior in C++
    const QCString & cppEntryBehavior();

    // set the entry behavior in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppEntryBehavior(const char * s);

    // return the exit behavior in C++
    const QCString & cppExitBehavior();

    // set the exit behavior in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppExitBehavior(const char * s);

    // return the activity in C++
    const QCString & cppDoActivity();

    // set the activity in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppDoActivity(const char * s);
#endif

#ifdef WITHJAVA
    // return the entry behavior in Java
    const QCString & javaEntryBehavior();

    // set the entry behavior in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaEntryBehavior(const char * s);

    // return the exit behavior in Java
    const QCString & javaExitBehavior();

    // set the exit behavior in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaExitBehavior(const char * s);

    // return the activity in Java
    const QCString & javaDoActivity();

    // set the activity in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaDoActivity(const char * s);
#endif

    // return the  indicate if the class is active
    bool isActive();

    // set if the class is active
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isActive(bool v);

    // return the specification
    UmlOperation * specification();

    // set the specification
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Specification(UmlOperation * v);

    // return the the referenced sub machine state or 0/null
    // if the state is not a sub machine state reference
    UmlState * reference();

    // set the referenced sub machine state (may be 0/null)
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Reference(UmlState * v);

    // returns the optional associated diagram
    UmlStateDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlStateDiagram * d);

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    StateBehavior _uml;

#ifdef WITHCPP
    StateBehavior _cpp;
#endif

#ifdef WITHJAVA
    StateBehavior _java;
#endif

    bool _active;

    UmlState * _reference;

    UmlOperation * _specification;

    UmlStateDiagram * _assoc_diagram;


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
