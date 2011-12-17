
#include "UmlPackage.h"

void UmlPackage::sort() {
  sortChildren();
}

int UmlPackage::orderWeight() {
  return 1;
}

