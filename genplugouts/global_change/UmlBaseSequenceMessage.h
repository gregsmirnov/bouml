#ifndef _UMLBASESEQUENCEMESSAGE_H
#define _UMLBASESEQUENCEMESSAGE_H


#include "aMessageKind.h"
#include <qcstring.h>
#include <qptrdict.h>
#include <qvector.h>
#include "UmlMessage.h"

class UmlFragmentCompartment;
class UmlClassInstanceReference;
class UmlFragment;

// this class manages messages in a sequence diagram
class UmlBaseSequenceMessage : public UmlMessage {
  public:
    // return kind of the message
    aMessageKind kind() const {
        return _kind;
    }

    // return the fragment compartment containing the message
    UmlFragmentCompartment * fragment() const {
        return _fragment;
    }

    // return when the message is sent (arbitrary unit)
    int sentAt() const {
        return _send_at;
    }

    // return when the message is received (arbitrary unit)
    int receivedAt() const {
        return _received_at;
    }

    // return the arguments of the operation, may be empty
    const QCString & arguments() const {
        return _args;
    }


  private:
    aMessageKind _kind;

    UmlFragmentCompartment * _fragment;

    int _x;

    int _send_at;

    int _received_at;

    QCString _args;

    // internal, don't call it
    void read_(const QPtrDict<UmlClassInstanceReference> & instances, const QVector<UmlFragment> & fragments);

  friend class UmlBaseSequenceDiagramDefinition;
};

#endif
