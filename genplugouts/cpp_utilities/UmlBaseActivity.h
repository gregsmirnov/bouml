#ifndef _UMLBASEACTIVITY_H
#define _UMLBASEACTIVITY_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlActivity;
class UmlClassView;
class UmlOperation;
class UmlActivityDiagram;

class UmlBaseActivity : public UmlItem {
  public:
    //  returns a new activity named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlActivity * create(UmlClassView * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivity(void * id, const QCString & s) : UmlItem(id, s) {
    }


  public:
    // return the pre condition
    const QCString & preCondition();

    // set the pre condition
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PreCondition(const char * v);

    // return the post condition
    const QCString & postCondition();

    // set the post condition
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PostCondition(const char * v);

#ifdef WITHCPP
    // return the pre condition in C++
    const QCString & cppPreCondition();

    // set the pre condition in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppPreCondition(const char * v);

    // return the post condition in C++
    const QCString & cppPostCondition();

    // set the post condition in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppPostCondition(const char * v);
#endif

#ifdef WITHJAVA
    // return the pre condition in Java
    const QCString & javaPreCondition();

    // set the pre condition in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaPreCondition(const char * v);

    // return the post condition in Java
    const QCString & javaPostCondition();

    // set the post condition in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaPostCondition(const char * v);
#endif

    // return the  return the isReadOnly attribute, if TRUE the activity must not made any changes to variables ouside the activity or to objects.
    bool isReadOnly();

    // set the isReadOnly attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReadOnly(bool v);

    // return the  return the isSingleExecution attribute, if TRUE all invocations of the activity are handle by the same execution
    bool isSingleExecution();

    // set the isSingleExecution attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isSingleExecution(bool v);

    // return the specification
    UmlOperation * specification();

    // set the specification
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Specification(UmlOperation * v);

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
    QCString _pre_condition;

    QCString _post_condition;

#ifdef WITHCPP
    QCString _cpp_pre_condition;

    QCString _cpp_post_condition;
#endif

#ifdef WITHJAVA
    QCString _java_pre_condition;

    QCString _java_post_condition;
#endif

    bool _read_only;

    bool _single_execution;

    UmlActivityDiagram * _assoc_diagram;


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif


  private:
    UmlOperation * _specification;

};

#endif
