#ifndef _UMLACTIVITYCONTROLNODECLASSES_H
#define _UMLACTIVITYCONTROLNODECLASSES_H


#include "UmlBaseActivityControlNodeClasses.h"
#include <qcstring.h>

class UmlInitialActivityNode : public UmlBaseInitialActivityNode {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlInitialActivityNode(void * id, const QCString & s) : UmlBaseInitialActivityNode(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

};

class UmlFlowFinalActivityNode : public UmlBaseFlowFinalActivityNode {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlFlowFinalActivityNode(void * id, const QCString & s) : UmlBaseFlowFinalActivityNode(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

};

class UmlActivityFinalActivityNode : public UmlBaseActivityFinalActivityNode {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityFinalActivityNode(void * id, const QCString & s) : UmlBaseActivityFinalActivityNode(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

};

class UmlDecisionActivityNode : public UmlBaseDecisionActivityNode {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlDecisionActivityNode(void * id, const QCString & s) : UmlBaseDecisionActivityNode(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

};

class UmlMergeActivityNode : public UmlBaseMergeActivityNode {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlMergeActivityNode(void * id, const QCString & s) : UmlBaseMergeActivityNode(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

};

class UmlForkActivityNode : public UmlBaseForkActivityNode {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlForkActivityNode(void * id, const QCString & s) : UmlBaseForkActivityNode(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

};

class UmlJoinActivityNode : public UmlBaseJoinActivityNode {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlJoinActivityNode(void * id, const QCString & s) : UmlBaseJoinActivityNode(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

};

#endif
