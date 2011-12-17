#ifndef _UMLBASEACTIVITYCONTROLNODECLASSES_H
#define _UMLBASEACTIVITYCONTROLNODECLASSES_H


#include "UmlActivityControlNode.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlInitialActivityNode;
class UmlItem;
class UmlFlowFinalActivityNode;
class UmlActivityFinalActivityNode;
class UmlDecisionActivityNode;
class UmlMergeActivityNode;
class UmlForkActivityNode;
class UmlJoinActivityNode;

class UmlBaseInitialActivityNode : public UmlActivityControlNode {
  public:
    //  returns a new initial activity control node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlInitialActivityNode * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseInitialActivityNode(void * id, const QCString & s) : UmlActivityControlNode(id, s) {
    }

};

class UmlBaseFlowFinalActivityNode : public UmlActivityControlNode {
  public:
    //  returns a new flow final activity control node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlFlowFinalActivityNode * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseFlowFinalActivityNode(void * id, const QCString & s) : UmlActivityControlNode(id, s) {
    }

};

class UmlBaseActivityFinalActivityNode : public UmlActivityControlNode {
  public:
    //  returns a new activity final activity control node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlActivityFinalActivityNode * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivityFinalActivityNode(void * id, const QCString & s) : UmlActivityControlNode(id, s) {
    }

};

class UmlBaseDecisionActivityNode : public UmlActivityControlNode {
  public:
    //  returns a new decision activity control node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlDecisionActivityNode * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseDecisionActivityNode(void * id, const QCString & s) : UmlActivityControlNode(id, s) {
    }

};

class UmlBaseMergeActivityNode : public UmlActivityControlNode {
  public:
    //  returns a new merge activity control node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlMergeActivityNode * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseMergeActivityNode(void * id, const QCString & s) : UmlActivityControlNode(id, s) {
    }

};

class UmlBaseForkActivityNode : public UmlActivityControlNode {
  public:
    //  returns a new fork activity control node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlForkActivityNode * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseForkActivityNode(void * id, const QCString & s) : UmlActivityControlNode(id, s) {
    }

};

class UmlBaseJoinActivityNode : public UmlActivityControlNode {
  public:
    //  returns a new join activity control node named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlJoinActivityNode * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseJoinActivityNode(void * id, const QCString & s) : UmlActivityControlNode(id, s) {
    }

};

#endif
