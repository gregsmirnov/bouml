
#include "UmlCom.h"
#include "UmlBaseCollaborationMessage.h"
#include "UmlClassInstanceReference.h"

void UmlBaseCollaborationMessage::read_(const QPtrDict<UmlClassInstanceReference> & instances) {
  UmlBaseMessage::read_(instances);
  _rank = (int) UmlCom::read_unsigned();
  _hrank = UmlCom::read_string();
}

