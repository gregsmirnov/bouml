
#include "UmlRelation.h"

#include "UmlCom.h"
bool UmlRelation::check() {
  if (roleName() == "the") {
    UmlCom::trace("<font face=helvetica><b>already have relation <i>the</i>, remove it</b></font><br><hr><br>");
    return FALSE;
  }
  else
    return TRUE;
}

