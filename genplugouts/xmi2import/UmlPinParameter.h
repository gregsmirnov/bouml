#ifndef _UMLPINPARAMETER_H
#define _UMLPINPARAMETER_H


#include "UmlBasePinParameter.h"
#include <qcstring.h>

class UmlPinParameter : public UmlBasePinParameter {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlPinParameter(void * id, const QCString & s) : UmlBasePinParameter(id, s) {
    }

};

#endif
