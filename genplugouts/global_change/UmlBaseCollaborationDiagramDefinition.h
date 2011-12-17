#ifndef _UMLBASECOLLABORATIONDIAGRAMDEFINITION_H
#define _UMLBASECOLLABORATIONDIAGRAMDEFINITION_H


// to avoid compiler warning
#include "UmlCollaborationMessage.h"
#include "UmlClassInstanceReference.h"

#include <qvector.h>

class UmlClassInstanceReference;
class UmlCollaborationMessage;

// this class manages collaboration diagram definition
class UmlBaseCollaborationDiagramDefinition {
  public:
    // return the instances present in the diagram
    const QVector<UmlClassInstanceReference> & instances() const {
        return _instances;
    }

    // return the messages present in the diagram,
    // ordonned following their rank
    const QVector<UmlCollaborationMessage> & messages() const {
        return _messages;
    }


  private:
    QVector<UmlClassInstanceReference> _instances;

    QVector<UmlCollaborationMessage> _messages;

    // internal, don't call it
    void read_();


  public:
    //  to avoid compiler warning, don't call it
     ~UmlBaseCollaborationDiagramDefinition();

  friend class UmlBaseCollaborationDiagram;
};

#endif
