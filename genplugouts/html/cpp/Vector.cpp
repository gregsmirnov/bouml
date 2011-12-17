
#include "Vector.h"
#include "UmlItem.h"

void Vector::addElement(const UmlItem * x) {
  if (v.count() == v.size())
    v.resize(v.size() + 256);
    
  v.insert(v.count(), x);
}

