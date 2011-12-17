
#include "UmlCom.h"
#include "UmlBaseSequenceDiagramDefinition.h"

static void qsort(UmlSequenceMessage ** v, int low, int high)
{
  if (low < high) {
    int lo = low;
    int hi = high + 1;
    int e = v[low]->sentAt();
    
    for (;;) {
      while ((++lo < hi) && (v[lo]->sentAt() <= e))
	;
      while (v[--hi]->sentAt() > e);
	;
      
      if (lo < hi) {
	UmlSequenceMessage * x = v[lo];
	
	v[lo] = v[hi];
	v[hi] = x;
      }
      else
	break;
    }
    
    UmlSequenceMessage * x = v[low];
	
    v[low] = v[hi];
    v[hi] = x;
    
    qsort(v, low, hi - 1);
    qsort(v, hi + 1, high);
  }
}
void UmlBaseSequenceDiagramDefinition::read_() {
  QPtrDict<UmlClassInstanceReference> instances;
  unsigned n;
  unsigned rank;

  n = UmlCom::read_unsigned();
  _fragments.resize(n);
  for (rank = 0; rank != n; rank += 1) {
    UmlFragment * f = new UmlFragment();

    _fragments.insert(rank, f);
    f->read_();
  }
  _fragments.setAutoDelete(TRUE);
  UmlBaseFragment::compute_container_(_fragments);

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
  if (n != 0) {
    for (rank = 0; rank != n; rank += 1) {
      UmlSequenceMessage * m = new UmlSequenceMessage();

      _messages.insert(rank, m);
      m->read_(instances, _fragments);
    }
    ::qsort(_messages.data(), 0, n - 1);
    _messages.setAutoDelete(TRUE);
  }

  n = _fragments.count();
  for (rank = 0; rank != n; rank += 1)
    _fragments.at(rank)->read_covered_(instances);


  while (UmlCom::read_bool()) {
    QCString s = UmlCom::read_string();
    int x = (int) UmlCom::read_unsigned();
    int y = (int) UmlCom::read_unsigned();
    int w = (int) UmlCom::read_unsigned();
    int h = (int) UmlCom::read_unsigned();
    UmlFragmentCompartment * cp = UmlBaseFragment::get_container_(x, y, w, h, _fragments);

    if (cp != 0) cp->add_text_(s);
  }

  while (UmlCom::read_bool()) {
    QCString s = UmlCom::read_string();
    int x = (int) UmlCom::read_unsigned();
    int y = (int) UmlCom::read_unsigned();
    int w = (int) UmlCom::read_unsigned();
    int h = (int) UmlCom::read_unsigned();
    UmlFragmentCompartment * cp = UmlBaseFragment::get_container_(x, y, w, h, _fragments);

    if (cp != 0) cp->add_cont_(s, y + h/2);
  }
}

 UmlBaseSequenceDiagramDefinition::~UmlBaseSequenceDiagramDefinition() {
}

