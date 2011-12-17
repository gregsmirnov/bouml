#ifndef _UMLBASEACTIVITYACTIONCLASSES_H
#define _UMLBASEACTIVITYACTIONCLASSES_H


#include "UmlActivityAction.h"
#include "anItemKind.h"
#include <qcstring.h>
#include "UmlOnSignalAction.h"
#include "UmlAccessVariableValueAction.h"

class UmlSendObjectAction;
class UmlItem;
class UmlUnmarshallAction;
class UmlSendSignalAction;
class UmlBroadcastSignalAction;
class UmlValueSpecificationAction;
class UmlOpaqueAction;
class UmlAcceptEventAction;
class UmlCallOperationAction;
class UmlOperation;
class UmlCallBehaviorAction;
class UmlClearVariableValueAction;
class UmlReadVariableValueAction;
class UmlWriteVariableValueAction;
class UmlAddVariableValueAction;
class UmlRemoveVariableValueAction;
class UmlAcceptCallAction;
class UmlReplyAction;
class UmlCreateObjectAction;
class UmlDestroyObjectAction;
class UmlTestIdentityAction;
class UmlRaiseExceptionAction;
class UmlReduceAction;

class UmlBaseSendObjectAction : public UmlActivityAction {
  public:
    //  returns a new send object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlSendObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseSendObjectAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }

};

class UmlBaseUnmarshallAction : public UmlActivityAction {
  public:
    //  returns a new unmarshall action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlUnmarshallAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseUnmarshallAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }

};

class UmlBaseSendSignalAction : public UmlOnSignalAction {
  public:
    //  returns a new send signal action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlSendSignalAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseSendSignalAction(void * id, const QCString & s) : UmlOnSignalAction(id, s) {
    }

};

class UmlBaseBroadcastSignalAction : public UmlOnSignalAction {
  public:
    //  returns a new broadcast signal action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlBroadcastSignalAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseBroadcastSignalAction(void * id, const QCString & s) : UmlOnSignalAction(id, s) {
    }

};

class UmlBaseValueSpecificationAction : public UmlActivityAction {
  public:
    //  returns a new value specification action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlValueSpecificationAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseValueSpecificationAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the value
    const QCString & value();

    // set the value
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Value(const char * v);

#ifdef WITHCPP
    // return the value in C++
    const QCString & cppValue();

    // set the value in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppValue(const char * v);
#endif

#ifdef WITHJAVA
    // return the value in Java
    const QCString & javaValue();

    // set the value in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaValue(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QCString _value;

#ifdef WITHCPP
    QCString _cpp_value;
#endif

#ifdef WITHJAVA
    QCString _java_value;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

class UmlBaseOpaqueAction : public UmlActivityAction {
  public:
    //  returns a new opaque action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlOpaqueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseOpaqueAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the behavior
    const QCString & behavior();

    // set the behavior
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Behavior(const char * v);

#ifdef WITHCPP
    // return the behavior in C++
    const QCString & cppBehavior();

    // set the behavior in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppBehavior(const char * v);
#endif

#ifdef WITHJAVA
    // return the behavior in Java
    const QCString & javaBehavior();

    // set the behavior in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaBehavior(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QCString _behavior;

#ifdef WITHCPP
    QCString _cpp_behavior;
#endif

#ifdef WITHJAVA
    QCString _java_behavior;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

class UmlBaseAcceptEventAction : public UmlActivityAction {
  public:
    //  returns a new accept event action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAcceptEventAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAcceptEventAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isUnmarshall attribute, if TRUE there are multiple output pins for attributes or the event.
    bool isUnmarshall();

    // set the isUnmarshall attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isUnmarshall(bool v);

    // return the  return the isTimeEvent attribute, if TRUE the event is a time event
    bool isTimeEvent();

    // set the isTimeEvent attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isTimeEvent(bool v);

    // return the trigger
    const QCString & trigger();

    // set the trigger
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Trigger(const char * v);

#ifdef WITHCPP
    // return the trigger in C++
    const QCString & cppTrigger();

    // set the trigger in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppTrigger(const char * v);
#endif

#ifdef WITHJAVA
    // return the trigger in Java
    const QCString & javaTrigger();

    // set the trigger in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaTrigger(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    bool _unmarshall;

    bool _timeevent;

    QCString _trigger;

#ifdef WITHCPP
    QCString _cpp_trigger;
#endif

#ifdef WITHJAVA
    QCString _java_trigger;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

class UmlBaseCallOperationAction : public UmlActivityAction {
  public:
    //  returns a new call operation action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCallOperationAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseCallOperationAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior
    bool isSynchronous();

    // set the isSynchronous attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isSynchronous(bool v);

    // return the operation
    UmlOperation * operation();

    // set the operation
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Operation(UmlOperation * v);


  private:
    bool _synchronous;

    UmlOperation * _operation;


  protected:
    virtual void read_uml_();

};

class UmlBaseCallBehaviorAction : public UmlActivityAction {
  public:
    //  returns a new call behavior action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCallBehaviorAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseCallBehaviorAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior
    bool isSynchronous();

    // set the isSynchronous attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isSynchronous(bool v);

    // return the behavior, may be an activity or a state machine
    UmlItem * behavior();

    // set the behavior
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Behavior(UmlItem * v);


  private:
    bool _synchronous;

    UmlItem * _behavior;


  protected:
    virtual void read_uml_();

};

class UmlBaseClearVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new clear variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlClearVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseClearVariableValueAction(void * id, const QCString & s) : UmlAccessVariableValueAction(id, s) {
    }

};

class UmlBaseReadVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new read variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReadVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReadVariableValueAction(void * id, const QCString & s) : UmlAccessVariableValueAction(id, s) {
    }

};

class UmlBaseWriteVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new write variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlWriteVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseWriteVariableValueAction(void * id, const QCString & s) : UmlAccessVariableValueAction(id, s) {
    }

};

class UmlBaseAddVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new add variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAddVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAddVariableValueAction(void * id, const QCString & s) : UmlAccessVariableValueAction(id, s) {
    }


  public:
    // return the  return the isReplaceAll attribute, if TRUE existing values of the variable must be removed before adding the new value
    bool isReplaceAll();

    // set the isReplaceAll attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReplaceAll(bool v);


  private:
    bool _replace_all;


  protected:
    virtual void read_uml_();

};

class UmlBaseRemoveVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new remove variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRemoveVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseRemoveVariableValueAction(void * id, const QCString & s) : UmlAccessVariableValueAction(id, s) {
    }


  public:
    // return the  return the isRemoveDuplicates attribute, if TRUE remove duplicates of the value if non-unique
    bool isRemoveDuplicates();

    // set the isRemoveDuplicates attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isRemoveDuplicates(bool v);


  private:
    bool _remove_duplicates;


  protected:
    virtual void read_uml_();

};

class UmlBaseAcceptCallAction : public UmlActivityAction {
  public:
    //  returns a new accept call action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAcceptCallAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAcceptCallAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the trigger
    const QCString & trigger();

    // set the trigger
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Trigger(const char * v);

#ifdef WITHCPP
    // return the trigger in C++
    const QCString & cppTrigger();

    // set the trigger in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppTrigger(const char * v);
#endif

#ifdef WITHJAVA
    // return the trigger in Java
    const QCString & javaTrigger();

    // set the trigger in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaTrigger(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QCString _trigger;

#ifdef WITHCPP
    QCString _cpp_trigger;
#endif

#ifdef WITHJAVA
    QCString _java_trigger;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

class UmlBaseReplyAction : public UmlActivityAction {
  public:
    //  returns a new reply action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReplyAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReplyAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the replyToCall trigger
    const QCString & replyToCall();

    // set the replyToCall trigger
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_ReplyToCall(const char * v);

#ifdef WITHCPP
    // return the replyToCall trigger in C++
    const QCString & cppReplyToCall();

    // set the replyToCall trigger in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppReplyToCall(const char * v);
#endif

#ifdef WITHJAVA
    // return the replyToCall trigger in Java
    const QCString & javaReplyToCall();

    // set the replyToCall trigger in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaReplyToCall(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QCString _trigger;

#ifdef WITHCPP
    QCString _cpp_trigger;
#endif

#ifdef WITHJAVA
    QCString _java_trigger;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

class UmlBaseCreateObjectAction : public UmlActivityAction {
  public:
    //  returns a new create object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCreateObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseCreateObjectAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the classifier
    const QCString & classifier();

    // set the classifier
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Classifier(const char * v);

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QCString _classifier;


  protected:
    virtual void read_uml_();

};

class UmlBaseDestroyObjectAction : public UmlActivityAction {
  public:
    //  returns a new destroy object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlDestroyObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseDestroyObjectAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isDestroyLinks attribute.
    bool isDestroyLinks();

    // set the isDestroyLinks attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isDestroyLinks(bool v);

    // return the  return the isDestroyOwnedObjects attribute
    bool isDestroyOwnedObjects();

    // set the isDestroyOwnedObjects attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isDestroyOwnedObjects(bool v);


  private:
    bool _links;

    bool _owned_objects;


  protected:
    virtual void read_uml_();

};

class UmlBaseTestIdentityAction : public UmlActivityAction {
  public:
    //  returns a new test identity action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlTestIdentityAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseTestIdentityAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }

};

class UmlBaseRaiseExceptionAction : public UmlActivityAction {
  public:
    //  returns a new raise exception action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRaiseExceptionAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseRaiseExceptionAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }

};

class UmlBaseReduceAction : public UmlActivityAction {
  public:
    //  returns a new reduce action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReduceAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReduceAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isOrdered attribute
    bool isOrdered();

    // set the isOrdered attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isOrdered(bool v);

    // return the reducer, may be an activity or a state machine
    UmlItem * reducer();

    // set the reducer
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Reducer(UmlItem * v);


  private:
    bool _ordered;

    UmlItem * _reducer;


  protected:
    virtual void read_uml_();

};

#endif
