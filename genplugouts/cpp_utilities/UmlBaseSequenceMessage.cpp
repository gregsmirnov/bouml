
#include "UmlCom.h"
#include "UmlBaseSequenceMessage.h"
#include "UmlFragmentCompartment.h"
#include "UmlClassInstanceReference.h"
#include "UmlFragment.h"

void UmlBaseSequenceMessage::read_(const QPtrDict<UmlClassInstanceReference> & instances, const QVector<UmlFragment> & fragments) {
  UmlBaseMessage::read_(instances);
  _kind = (aMessageKind) UmlCom::read_char();
  _args = UmlCom::read_string();
  _x = (int) UmlCom::read_unsigned();
  _send_at = (int) UmlCom::read_unsigned();
  _received_at = (int) UmlCom::read_unsigned();
  _fragment = UmlBaseFragment::get_container_(_x, _send_at, 1, 1, fragments);
}

