
#include "UmlTypeSpec.h"
#include "UmlClass.h"

const QCString & UmlTypeSpec::toString() const {
  return (type) ? type->name() : explicit_type;
}

