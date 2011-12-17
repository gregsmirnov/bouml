#ifndef _UMLBASEFLOW_H
#define _UMLBASEFLOW_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>
#include "FlowBehavior.h"

class UmlFlow;
class UmlActivityNode;

class UmlBaseFlow : public UmlItem {
  public:
    // returns the kind of the item
    virtual anItemKind kind();

    // Returns a new flow from 'start' to 'end'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...) return 0
    //  in C++ and produce a RuntimeException in Java
    
    static UmlFlow * create(UmlActivityNode * start, UmlActivityNode * end);

    // returns the 'end' object (the 'start' object is the parent of the flow) no set !
    virtual UmlActivityNode * target();

    // return the weight in OCL
    const QCString & weight();

    // set the weight in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Weight(const char * v);

    // return the guard in OCL
    const QCString & guard();

    // set the guard in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Guard(const char * v);

    // return the selection in OCL
    const QCString & selection();

    // set the selection in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Selection(const char * v);

    // return the transformation in OCL
    const QCString & transformation();

    // set the transformation in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Transformation(const char * v);

#ifdef WITHCPP
    // return the weight in C++
    const QCString & cppWeight();

    // set the weight in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppWeight(const char * v);

    // return the guard in C++
    const QCString & cppGuard();

    // set the guard in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppGuard(const char * v);

    // return the selection in C++
    const QCString & cppSelection();

    // set the selection in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppSelection(const char * v);

    // return the transformation in C++
    const QCString & cppTransformation();

    // set the transformation in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppTransformation(const char * v);
#endif

#ifdef WITHJAVA
    // return the weight in Java
    const QCString & javaWeight();

    // set the weight in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaWeight(const char * v);

    // return the guard in Java
    const QCString & javaGuard();

    // set the guard in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaGuard(const char * v);

    // return the selection in Java
    const QCString & javaSelection();

    // set the selection in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaSelection(const char * v);

    // return the transformation in Java
    const QCString & javaTransformation();

    // set the transformation in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaTransformation(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    UmlActivityNode * _target;

    FlowBehavior _uml;

#ifdef WITHCPP
    FlowBehavior _cpp;
#endif

#ifdef WITHJAVA
    FlowBehavior _java;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseFlow(void * id, const QCString & s) : UmlItem(id, s) {
    }

};

#endif
