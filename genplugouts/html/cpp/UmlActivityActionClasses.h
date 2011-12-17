#ifndef _UMLACTIVITYACTIONCLASSES_H
#define _UMLACTIVITYACTIONCLASSES_H


#include <qcstring.h>
#include "UmlBaseActivityActionClasses.h"

class UmlSendObjectAction : public UmlBaseSendObjectAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlSendObjectAction(void * id, const QCString & s) : UmlBaseSendObjectAction(id, s) {
    }

};

class UmlUnmarshallAction : public UmlBaseUnmarshallAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlUnmarshallAction(void * id, const QCString & s) : UmlBaseUnmarshallAction(id, s) {
    }

};

class UmlSendSignalAction : public UmlBaseSendSignalAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlSendSignalAction(void * id, const QCString & s) : UmlBaseSendSignalAction(id, s) {
    }

};

class UmlBroadcastSignalAction : public UmlBaseBroadcastSignalAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBroadcastSignalAction(void * id, const QCString & s) : UmlBaseBroadcastSignalAction(id, s) {
    }

};

class UmlValueSpecificationAction : public UmlBaseValueSpecificationAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlValueSpecificationAction(void * id, const QCString & s) : UmlBaseValueSpecificationAction(id, s) {
    }

};

class UmlOpaqueAction : public UmlBaseOpaqueAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlOpaqueAction(void * id, const QCString & s) : UmlBaseOpaqueAction(id, s) {
    }

};

class UmlAcceptEventAction : public UmlBaseAcceptEventAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAcceptEventAction(void * id, const QCString & s) : UmlBaseAcceptEventAction(id, s) {
    }

};

class UmlCallOperationAction : public UmlBaseCallOperationAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCallOperationAction(void * id, const QCString & s) : UmlBaseCallOperationAction(id, s) {
    }

};

class UmlCallBehaviorAction : public UmlBaseCallBehaviorAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCallBehaviorAction(void * id, const QCString & s) : UmlBaseCallBehaviorAction(id, s) {
    }

};

class UmlClearVariableValueAction : public UmlBaseClearVariableValueAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlClearVariableValueAction(void * id, const QCString & s) : UmlBaseClearVariableValueAction(id, s) {
    }

};

class UmlReadVariableValueAction : public UmlBaseReadVariableValueAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReadVariableValueAction(void * id, const QCString & s) : UmlBaseReadVariableValueAction(id, s) {
    }

};

class UmlWriteVariableValueAction : public UmlBaseWriteVariableValueAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlWriteVariableValueAction(void * id, const QCString & s) : UmlBaseWriteVariableValueAction(id, s) {
    }

};

class UmlAddVariableValueAction : public UmlBaseAddVariableValueAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAddVariableValueAction(void * id, const QCString & s) : UmlBaseAddVariableValueAction(id, s) {
    }

};

class UmlRemoveVariableValueAction : public UmlBaseRemoveVariableValueAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRemoveVariableValueAction(void * id, const QCString & s) : UmlBaseRemoveVariableValueAction(id, s) {
    }

};

class UmlAcceptCallAction : public UmlBaseAcceptCallAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAcceptCallAction(void * id, const QCString & s) : UmlBaseAcceptCallAction(id, s) {
    }

};

class UmlReplyAction : public UmlBaseReplyAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReplyAction(void * id, const QCString & s) : UmlBaseReplyAction(id, s) {
    }

};

class UmlCreateObjectAction : public UmlBaseCreateObjectAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCreateObjectAction(void * id, const QCString & s) : UmlBaseCreateObjectAction(id, s) {
    }

};

class UmlDestroyObjectAction : public UmlBaseDestroyObjectAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlDestroyObjectAction(void * id, const QCString & s) : UmlBaseDestroyObjectAction(id, s) {
    }

};

class UmlTestIdentityAction : public UmlBaseTestIdentityAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlTestIdentityAction(void * id, const QCString & s) : UmlBaseTestIdentityAction(id, s) {
    }

};

class UmlRaiseExceptionAction : public UmlBaseRaiseExceptionAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRaiseExceptionAction(void * id, const QCString & s) : UmlBaseRaiseExceptionAction(id, s) {
    }

};

class UmlReduceAction : public UmlBaseReduceAction {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReduceAction(void * id, const QCString & s) : UmlBaseReduceAction(id, s) {
    }

};

#endif
