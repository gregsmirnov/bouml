
#include "UmlCom.h"
#include "UmlBaseUseCaseReference.h"
#include "UmlUseCase.h"
#include "UmlFragmentCompartment.h"
#include "UmlSubject.h"
#include "UmlFragment.h"

void UmlBaseUseCaseReference::read_(const QVector<UmlFragment> & fragments, const QVector<UmlSubject> & subjects) {
  _use_case = (UmlUseCase *) UmlBaseItem::read_();
  _x = (int) UmlCom::read_unsigned();
  _y = (int) UmlCom::read_unsigned();
  _w = (int) UmlCom::read_unsigned();
  _h = (int) UmlCom::read_unsigned();
  _fragment = UmlBaseFragment::get_container_(_x, _y, _w, _h, fragments);
  _subject = UmlBaseSubject::get_container_(_x, _y, _w, _h, subjects);
}

