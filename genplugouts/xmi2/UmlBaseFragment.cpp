
#include "UmlCom.h"
#include "UmlBaseFragment.h"
#include "UmlFragmentCompartment.h"
#include "UmlDiagram.h"
#include "UmlClassInstanceReference.h"
#include "UmlFragment.h"

void UmlBaseFragment::read_() {
  _name = UmlCom::read_string();
  _x = (int) UmlCom::read_unsigned();
  _y = (int) UmlCom::read_unsigned();
  _w = (int) UmlCom::read_unsigned();
  _h = (int) UmlCom::read_unsigned();

  unsigned n = UmlCom::read_unsigned();

  _compartments.resize(n);
  for (unsigned rank = 0; rank != n; rank += 1) {
    UmlFragmentCompartment * fc = new UmlFragmentCompartment();

    _compartments.insert(rank, fc);
    fc->read_(this, rank);
  }
  _compartments.setAutoDelete(TRUE);
  _container = 0;
  _refer = (UmlDiagram *) UmlBaseItem::read_();
  _arguments = UmlCom::read_string();
}

int UmlBaseFragment::vcenter_(int rank) const {
  int t = (rank == 0) ? _y : _compartments[rank - 1]->b();

  return (t + _compartments[rank]->b())/2;
}

void UmlBaseFragment::read_covered_(QPtrDict<UmlClassInstanceReference> & instances) {
  unsigned n = UmlCom::read_unsigned();
  unsigned rank;

  _covered.resize(n);
  for (rank = 0; rank != n; rank += 1)
    _covered.insert(rank, instances[(void *) UmlCom::read_unsigned()]);
}

UmlFragmentCompartment * UmlBaseFragment::get_container_(int x, int y, int w, int h, const QVector<UmlFragment> & fragments)
{
  UmlFragmentCompartment * r = 0;
  int nf = fragments.count();
  int frank;

  for (frank = 0; frank != nf; frank += 1) {
    const UmlBaseFragment * f = fragments[frank];

    if ((x > f->_x) && (y > f->_y) &&
        ((x + w) < (f->_x + f->_w)) && ((y + h) < (f->_y + f->_h))) {
      int y0 = f->_y;
      int nfc = f->_compartments.count();
      int fcrank;

      for (fcrank = 0; fcrank != nfc; fcrank += 1) {
        UmlFragmentCompartment * fc = f->_compartments[fcrank];

        if ((y > y0) &&
            ((y + h) < fc->b()) &&
            ((r == 0) || fc->smaller(r))) {
          r = fc;
          break;
        }
        y0 = fc->b();
      }
    }
  }
  return r;
}

void UmlBaseFragment::compute_container_(QVector<UmlFragment> & fragments)
{
  int rank = fragments.count();

  while (rank-- != 0) {
    UmlBaseFragment * f = fragments[rank];
    UmlFragmentCompartment * fc = get_container_(f->_x, f->_y, f->_w, f->_h, fragments);

    if (fc != 0) {
      f->_container = fc;
      fc->add_contained_((UmlFragment *) f);
    }
  }
}

 UmlBaseFragment::~UmlBaseFragment() {
}

