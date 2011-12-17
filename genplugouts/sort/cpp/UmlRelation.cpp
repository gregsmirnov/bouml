
#include "UmlRelation.h"

int UmlRelation::orderWeight() {
  switch (relationKind()) {
  case aGeneralisation:
    return 11;
  case aDependency:
    return 12;
  default:
    return 0;
  }
}

