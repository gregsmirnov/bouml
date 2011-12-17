
#include "UmlCom.h"
#include "UmlBaseSubject.h"
#include "UmlSubject.h"

void UmlBaseSubject::read_() {
  _name = UmlCom::read_string();
  _x = (int) UmlCom::read_unsigned();
  _y = (int) UmlCom::read_unsigned();
  _w = (int) UmlCom::read_unsigned();
  _h = (int) UmlCom::read_unsigned();
}

UmlSubject * UmlBaseSubject::get_container_(int x, int y, int w, int h, const QVector<UmlSubject> & subjects)
{
  int rank = subjects.count();

  while (rank-- != 0) {
    UmlBaseSubject * s = subjects[rank];

    if ((x > s->_x) && (y > s->_y) &&
        ((x + w) < (s->_x + s->_w)) && ((y + h) < (s->_y + s->_h)))
      return (UmlSubject  *) s;
  }
  return 0;
}

