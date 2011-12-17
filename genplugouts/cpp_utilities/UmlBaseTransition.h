#ifndef _UMLBASETRANSITION_H
#define _UMLBASETRANSITION_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>
#include "TransitionBehavior.h"

class UmlTransition;

class UmlBaseTransition : public UmlItem {
  public:
    // returns the kind of the item
    virtual anItemKind kind();

    // Returns a new Transition from 'start' to 'end'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...) return 0
    //  in C++ and produce a RuntimeException in Java
    
    static UmlTransition * create(UmlItem * start, UmlItem * end);

    // returns the 'end' object (the 'start' object is the parent of the transition) no set !
    virtual UmlItem * target();

    // return the  return if the transition is internal or external,
    // only self transitions can't be external
    bool isExternal();

    // set if the transition is internal or external,
    // only a self transition may be set internal
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IsExternal(bool v);

    // return the trigger in OCL
    const QCString & trigger();

    // set the trigger in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Trigger(const char * s);

    // return the guard in OCL
    const QCString & guard();

    // set the guard in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Guard(const char * s);

    // return the activity in OCL
    const QCString & activity();

    // set the activity in OCL
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Activity(const char * s);

#ifdef WITHCPP
    // return the trigger in C++
    const QCString & cppTrigger();

    // set the trigger in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppTrigger(const char * s);

    // return the guard in C++
    const QCString & cppGuard();

    // set the guard in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppGuard(const char * s);

    // return the activity in C++
    const QCString & cppActivity();

    // set the activity in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppActivity(const char * s);
#endif

#ifdef WITHJAVA
    // return the trigger in Java
    const QCString & javaTrigger();

    // set the trigger in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaTrigger(const char * s);

    // return the guard in Java
    const QCString & javaGuard();

    // set the guard in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaGuard(const char * s);

    // return the activity in Java
    const QCString & javaActivity();

    // set the activity in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaActivity(const char * s);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    UmlItem * _target;


  public:
    bool _is_external;


  private:
    TransitionBehavior _uml;

#ifdef WITHCPP
    TransitionBehavior _cpp;
#endif

#ifdef WITHJAVA
    TransitionBehavior _java;
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
     UmlBaseTransition(void * id) : UmlItem(id, "") {
    }

};

#endif
