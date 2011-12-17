
#include "UmlItem.h"

#include "UmlCom.h"
 UmlItem::~UmlItem() {
}

void UmlItem::sort() {
  UmlCom::trace("target not allowed, must be a <i>package</i>, any <i>view</i> or a <i>use case</i>");
}

int UmlItem::orderWeight() {
  return 0;
}

void UmlItem::sortChildren() {
  const QVector<UmlItem> & qv = children();
  
  if (!qv.isEmpty()) {
    unsigned sz = qv.size();
    VectorOfUmlItem v;
    unsigned u;
    
    // copy
    v.resize(sz);
    for (u = 0; u != sz; u += 1)
      v.insert(u, qv[u]);
    
    // sort in memory
    v.sort();
    
    // update browser
    UmlItem * previous = 0;
    
    for (u = 0; u != sz; u += 1) {
      v[u]->moveAfter(previous);
      previous = v[u];
    }
  }
}

int VectorOfUmlItem::compareItems(QCollection::Item d1, QCollection::Item d2) {
  UmlItem * e1 = (UmlItem *) d1;
  UmlItem * e2 = (UmlItem *) d2;
  int w1 = e1->orderWeight();
  int w2 = e2->orderWeight();
  
  if (w1 != w2)
    return w1 - w2;
  else if (e1->name() > (const char *) e2->name())
    return 1;
  else if (e1->name() < (const char *) e2->name())
    return -1;
  else
    return 0;
}

