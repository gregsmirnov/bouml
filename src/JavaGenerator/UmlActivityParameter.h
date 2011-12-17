#ifndef _UMLACTIVITYPARAMETER_H
#define _UMLACTIVITYPARAMETER_H


#include "UmlBaseActivityParameter.h"
#include <qcstring.h>

class UmlActivityParameter : public UmlBaseActivityParameter {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityParameter(void * id, const QCString & s) : UmlBaseActivityParameter(id, s) {
    }

};

#endif
