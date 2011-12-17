#ifndef _UMLPSEUDOSTATE_H
#define _UMLPSEUDOSTATE_H


#include <qcstring.h>
#include "UmlItem.h"
#include "UmlStateItem.h"

class UmlPseudoState : public UmlStateItem, public UmlItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlPseudoState(void * id, const QCString & s) : UmlItem(id, s) {
    }

};

#endif
