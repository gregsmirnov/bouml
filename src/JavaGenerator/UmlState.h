#ifndef _UMLSTATE_H
#define _UMLSTATE_H


#include "UmlBaseState.h"
#include "UmlStateItem.h"
#include <qcstring.h>

class FileOut;

class UmlState : public UmlBaseState, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
 UmlState(void * id, const QCString & s) : UmlBaseState(id, s) {
}


};

#endif
