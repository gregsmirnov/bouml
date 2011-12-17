#ifndef _UMLACTIVITYPARTITION_H
#define _UMLACTIVITYPARTITION_H


#include "UmlBaseActivityPartition.h"
#include "UmlActivityItem.h"
#include <qcstring.h>

class FileOut;

class UmlActivityPartition : public UmlBaseActivityPartition, public UmlActivityItem {
  public:
    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

    //goes among tree to memorize incoming flow
    
    virtual void memo_incoming_flow();

    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityPartition(void * id, const QCString & s) : UmlBaseActivityPartition(id, s) {
    }

};

#endif
