#ifndef _UMLACTIVITYPARTITION_H
#define _UMLACTIVITYPARTITION_H


#include <qcstring.h>
#include "UmlBaseActivityPartition.h"
#include "UmlActivityItem.h"

class UmlActivityPartition : public UmlBaseActivityPartition, public UmlActivityItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityPartition(void * id, const QCString & s) : UmlBaseActivityPartition(id, s) {
    }

};

#endif
