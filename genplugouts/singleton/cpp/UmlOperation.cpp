
#include "UmlOperation.h"

#include "UmlCom.h"
bool UmlOperation::check() {
  if (name() == parent()->name()) {
    UmlCom::trace("<font face=helvetica><b>already have <i>constructor</i>, remove it</b></font><br><hr><br>");
    return FALSE;
  }
  else if (name() == "instance") {
    UmlCom::trace("<font face=helvetica><b>already have operation <i>instance</i>, remove it</b></font><br><hr><br>");
    return FALSE;
  }
  else
    return TRUE;
}

