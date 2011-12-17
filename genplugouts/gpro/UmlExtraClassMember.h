#ifndef _UMLEXTRACLASSMEMBER_H
#define _UMLEXTRACLASSMEMBER_H


#include "UmlBaseExtraClassMember.h"
#include <qcstring.h>

// This class allows to manage extra class member, It allows to insert
// C++ pre-processor directive (even they may be placed in the other member
// definition/declaration), etc ...
//
// You can modify it as you want (except the constructor)

class UmlExtraClassMember : public UmlBaseExtraClassMember {
  public:
    UmlExtraClassMember(void * id, const QCString & n) : UmlBaseExtraClassMember(id, n) {};

};

#endif
