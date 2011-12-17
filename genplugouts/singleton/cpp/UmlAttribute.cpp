
#include "UmlAttribute.h"

#include "UmlCom.h"
bool UmlAttribute::check() {
  if (name() == "the") {
    UmlCom::trace("<font face=helvetica><b>already have attribute <i>the</i>, remove it</b></font><br><hr><br>");
    return FALSE;
  }
  else
    return TRUE;
}

