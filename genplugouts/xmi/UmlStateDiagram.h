#ifndef _UMLSTATEDIAGRAM_H
#define _UMLSTATEDIAGRAM_H


#include "UmlBaseStateDiagram.h"
#include "UmlStateItem.h"
#include <qcstring.h>

class UmlStateDiagram : public UmlBaseStateDiagram, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlStateDiagram(void * id, const QCString & s) : UmlBaseStateDiagram(id, s) {
    }

};

#endif
