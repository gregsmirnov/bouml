#ifndef _UMLACTIVITYPIN_H
#define _UMLACTIVITYPIN_H


#include "UmlBaseActivityPin.h"
#include <qcstring.h>

class UmlActivityPin : public UmlBaseActivityPin {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityPin(void * id, const QCString & s) : UmlBaseActivityPin(id, s) {
    }

};

#endif
