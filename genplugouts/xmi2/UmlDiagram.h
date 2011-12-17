#ifndef _UMLDIAGRAM_H
#define _UMLDIAGRAM_H


#include "UmlBaseDiagram.h"
#include <qcstring.h>

// This class allows to manage extra class member, mainly defined fot C++
// it allows to insert C++ pre-processor directive (even they may be placed
// in the other member definition/declaration), to declare friend
// operation/function etc...
//
// You can modify it as you want (except the constructor)
class UmlDiagram : public UmlBaseDiagram {
  public:
    UmlDiagram(void * id, const QCString & n) : UmlBaseDiagram(id, n) {};

};

#endif
