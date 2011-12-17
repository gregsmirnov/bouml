#ifndef _UMLBASEPARAMETER_H
#define _UMLBASEPARAMETER_H


#include <qcstring.h>
#include "UmlTypeSpec.h"
#include "aDirection.h"

//  Represent an operation's parameter

struct UmlBaseParameter {
    QCString name;

    QCString default_value;

    UmlTypeSpec type;

    aDirection dir;

};

#endif
