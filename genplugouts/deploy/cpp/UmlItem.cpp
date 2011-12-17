
#include "UmlItem.h"
#include "UmlDeploymentView.h"

#include "UmlCom.h"
 UmlItem::~UmlItem() {
}

void UmlItem::deploy() {
  UmlCom::trace("Error : must be applied on a <i>class view</i><br>");
}

void UmlItem::deploy(UmlDeploymentView *) {
}

int UmlItem::deployable() {
return 0;
}

