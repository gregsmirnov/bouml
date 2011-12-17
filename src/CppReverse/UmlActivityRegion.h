#ifndef _UMLACTIVITYREGION_H
#define _UMLACTIVITYREGION_H


#include "UmlBaseActivityRegion.h"
#include "UmlActivityItem.h"
#include <qcstring.h>

class UmlActivityRegion : public UmlBaseActivityRegion, public UmlActivityItem {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityRegion(void * id, const QCString & s) : UmlBaseActivityRegion(id, s) {
    }

};

#endif
