
#include "UmlCom.h"
#include "UmlBaseMessage.h"
#include "UmlClassInstanceReference.h"
#include "UmlOperation.h"

void UmlBaseMessage::read_(const QPtrDict<UmlClassInstanceReference> & instances) {
  _operation = (UmlOperation *) UmlBaseItem::read_();
  if (_operation == 0) _form = UmlCom::read_string();
  _from = instances[(void *) UmlCom::read_unsigned()];
  _to = instances[(void *) UmlCom::read_unsigned()];
}

