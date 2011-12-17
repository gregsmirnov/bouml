#ifndef _UMLPARAMETER_H
#define _UMLPARAMETER_H


#include "UmlBaseParameter.h"
#include <qcstring.h>

//  Represent an operation's parameter

struct UmlParameter : public UmlBaseParameter {
    QCString cpp(unsigned rank) const;

};

#endif
