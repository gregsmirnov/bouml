#ifndef _UMLFLOW_H
#define _UMLFLOW_H


#include "UmlBaseFlow.h"
#include "UmlActivityItem.h"
#include <qcstring.h>

class UmlFlow : public UmlBaseFlow, public UmlActivityItem {
  public:
     UmlFlow(void * id, const QCString & n) : UmlBaseFlow(id, n) {
    }

};

#endif
