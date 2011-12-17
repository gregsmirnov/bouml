#ifndef _UMLPARAMETERSET_H
#define _UMLPARAMETERSET_H


#include "UmlBaseParameterSet.h"
#include <qcstring.h>

class UmlParameterSet : public UmlBaseParameterSet {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlParameterSet(void * id, const QCString & s) : UmlBaseParameterSet(id, s) {
    }

};

#endif
