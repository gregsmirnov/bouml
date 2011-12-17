
#include "UmlBaseDiagram.h"

#include "UmlCom.h"
bool UmlBaseDiagram::saveIn(const QCString & s) const {
  UmlCom::send_cmd(_identifier, saveInCmd, s);
  return UmlCom::read_bool();
}

