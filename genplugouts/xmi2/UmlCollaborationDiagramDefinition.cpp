
#include "UmlCollaborationDiagramDefinition.h"
#include "FileOut.h"
#include "UmlItem.h"

UmlCollaborationDiagramDefinition::~UmlCollaborationDiagramDefinition() {
}

void UmlCollaborationDiagramDefinition::write(FileOut & out, UmlItem * diag) {
  UmlMessage::write_connectors(out, diag, 
			       (const QVector<UmlMessage>&) messages());
  UmlClassInstanceReference::write_lifelines(out, diag, instances());
  if (! messages().isEmpty()) {
    unsigned index = 0;
    
    UmlCollaborationMessage::write(out, diag, messages(), index);
  }
}

