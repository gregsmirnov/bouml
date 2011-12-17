#ifndef _UMLACTIVITYACTIONCLASSES_H
#define _UMLACTIVITYACTIONCLASSES_H


#include "UmlBaseActivityActionClasses.h"
#include <qcstring.h>

class FileOut;

class UmlSendObjectAction : public UmlBaseSendObjectAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlSendObjectAction(void * id, const QCString & s) : UmlBaseSendObjectAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlUnmarshallAction : public UmlBaseUnmarshallAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlUnmarshallAction(void * id, const QCString & s) : UmlBaseUnmarshallAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlSendSignalAction : public UmlBaseSendSignalAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlSendSignalAction(void * id, const QCString & s) : UmlBaseSendSignalAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlBroadcastSignalAction : public UmlBaseBroadcastSignalAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBroadcastSignalAction(void * id, const QCString & s) : UmlBaseBroadcastSignalAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlValueSpecificationAction : public UmlBaseValueSpecificationAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlValueSpecificationAction(void * id, const QCString & s) : UmlBaseValueSpecificationAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlOpaqueAction : public UmlBaseOpaqueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlOpaqueAction(void * id, const QCString & s) : UmlBaseOpaqueAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlAcceptEventAction : public UmlBaseAcceptEventAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAcceptEventAction(void * id, const QCString & s) : UmlBaseAcceptEventAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlCallOperationAction : public UmlBaseCallOperationAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCallOperationAction(void * id, const QCString & s) : UmlBaseCallOperationAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlCallBehaviorAction : public UmlBaseCallBehaviorAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCallBehaviorAction(void * id, const QCString & s) : UmlBaseCallBehaviorAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlClearVariableValueAction : public UmlBaseClearVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlClearVariableValueAction(void * id, const QCString & s) : UmlBaseClearVariableValueAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlReadVariableValueAction : public UmlBaseReadVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReadVariableValueAction(void * id, const QCString & s) : UmlBaseReadVariableValueAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlWriteVariableValueAction : public UmlBaseWriteVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlWriteVariableValueAction(void * id, const QCString & s) : UmlBaseWriteVariableValueAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlAddVariableValueAction : public UmlBaseAddVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAddVariableValueAction(void * id, const QCString & s) : UmlBaseAddVariableValueAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlRemoveVariableValueAction : public UmlBaseRemoveVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRemoveVariableValueAction(void * id, const QCString & s) : UmlBaseRemoveVariableValueAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlAcceptCallAction : public UmlBaseAcceptCallAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAcceptCallAction(void * id, const QCString & s) : UmlBaseAcceptCallAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlReplyAction : public UmlBaseReplyAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReplyAction(void * id, const QCString & s) : UmlBaseReplyAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlCreateObjectAction : public UmlBaseCreateObjectAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCreateObjectAction(void * id, const QCString & s) : UmlBaseCreateObjectAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlDestroyObjectAction : public UmlBaseDestroyObjectAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlDestroyObjectAction(void * id, const QCString & s) : UmlBaseDestroyObjectAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlTestIdentityAction : public UmlBaseTestIdentityAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlTestIdentityAction(void * id, const QCString & s) : UmlBaseTestIdentityAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlRaiseExceptionAction : public UmlBaseRaiseExceptionAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRaiseExceptionAction(void * id, const QCString & s) : UmlBaseRaiseExceptionAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

class UmlReduceAction : public UmlBaseReduceAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReduceAction(void * id, const QCString & s) : UmlBaseReduceAction(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

#endif
