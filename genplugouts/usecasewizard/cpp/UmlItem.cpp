
#include "UmlCom.h"
#include "UmlItem.h"

 UmlItem::~UmlItem() {
}

void UmlItem::wizard() {
  UmlCom::trace("<font face=helvetica><b>Use case wizard must be applied on a <i>use case</i></b></font><br><hr><br>");
}

