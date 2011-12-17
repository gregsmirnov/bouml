
#include "UmlSequenceDiagramDefinition.h"
#include "FileOut.h"
#include "UmlItem.h"

UmlSequenceDiagramDefinition::~UmlSequenceDiagramDefinition() {
}

void UmlSequenceDiagramDefinition::write(FileOut & out, UmlItem * diag) {
  UmlMessage::write_connectors(out, diag, 
			       (const QVector<UmlMessage>&) messages());
  UmlClassInstanceReference::write_lifelines(out, diag, instances());
  UmlSequenceMessage::write(out, diag, messages());

}

