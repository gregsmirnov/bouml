#ifndef _UMLBASEACTIVITYACTION_H
#define _UMLBASEACTIVITYACTION_H


#include "UmlActivityNode.h"
#include <qcstring.h>

class UmlDiagram;

class UmlBaseActivityAction : public UmlActivityNode {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivityAction(void * id, const QCString & s) : UmlActivityNode(id, s) {
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

    // returns the optional associated diagram
    UmlDiagram * associatedDiagram();

    // sets the associated diagram, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_AssociatedDiagram(UmlDiagram * d);

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

    UmlDiagram * _assoc_diagram;


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
