
#include "UmlItem.h"

#include "UmlCom.h"
 UmlItem::~UmlItem() {
}

void UmlItem::genpro() {
  UmlCom::trace("must be applied on a component");
}

