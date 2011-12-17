#ifndef _UMLPINPARAMETER_H
#define _UMLPINPARAMETER_H


#include "UmlBasePinParameter.h"
#include <qcstring.h>

class UmlPinParameter : public UmlBasePinParameter {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlPinParameter(void * id, const QCString & s) : UmlBasePinParameter(id, s) {
    }


  public:
    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

};

#endif
