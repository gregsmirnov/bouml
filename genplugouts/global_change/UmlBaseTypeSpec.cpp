
#include "UmlBaseTypeSpec.h"
#include "UmlClass.h"

const QCString & UmlBaseTypeSpec::toString() const {
  return (type) ? type->name() : explicit_type;
}

