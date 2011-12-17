
#include "UmlCom.h"
#include "UmlBaseCollaborationDiagramDefinition.h"

void UmlBaseCollaborationDiagramDefinition::read_() {
  QPtrDict<UmlClassInstanceReference> instances;
  unsigned n;
  unsigned rank;

  n = UmlCom::read_unsigned();
  _instances.resize(n);
  for (rank = 0; rank != n; rank += 1) {
    UmlClassInstanceReference * i = new UmlClassInstanceReference();

    _instances.insert(rank, i);
    instances.insert((void *) UmlCom::read_unsigned(), i);
    i->read_();
  }
  _instances.setAutoDelete(TRUE);

  n = UmlCom::read_unsigned();
  _messages.resize(n);
  for (rank = 0; rank != n; rank += 1) {
    UmlCollaborationMessage * m = new UmlCollaborationMessage();

    _messages.insert(rank, m);
    m->read_(instances);
  }
  _messages.setAutoDelete(TRUE);
}

 UmlBaseCollaborationDiagramDefinition::~UmlBaseCollaborationDiagramDefinition() {
}

