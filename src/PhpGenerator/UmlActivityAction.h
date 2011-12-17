#ifndef _UMLACTIVITYACTION_H
#define _UMLACTIVITYACTION_H


#include "UmlBaseActivityAction.h"
#include <qcstring.h>

class UmlActivityAction : public UmlBaseActivityAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityAction(void * id, const QCString & s) : UmlBaseActivityAction(id, s) {
    }

};

#endif
