#ifndef _UMLBASECOLLABORATIONMESSAGE_H
#define _UMLBASECOLLABORATIONMESSAGE_H


#include <qcstring.h>
#include <qptrdict.h>
#include "UmlMessage.h"

class UmlClassInstanceReference;

// this class manages messages in a collaboration diagram
class UmlBaseCollaborationMessage : public UmlMessage {
  public:
    // return the global rank of the message
    int rank() const {
        return _rank;
    }

    // return the hierarchical rank of the message
    const QCString & hrank() const {
        return _hrank;
    }


  private:
    int _rank;

    QCString _hrank;

    // internal, don't call it
    void read_(const QPtrDict<UmlClassInstanceReference> & instances);

  friend class UmlBaseCollaborationDiagramDefinition;
};

#endif
