
#include "UmlItem.h"

#include "UmlCom.h"
 UmlItem::~UmlItem() {
}

void UmlItem::singleton() {
  UmlCom::trace("<font face=helvetica><b>Singleton must be applied on a <i>class</i></b></font><br><hr><br>");
}

