#ifndef _UMLPARAMETERSET_H
#define _UMLPARAMETERSET_H


#include "UmlBaseParameterSet.h"
#include "UmlActivityItem.h"
#include <qcstring.h>

class FileOut;

class UmlParameterSet : public UmlBaseParameterSet, public UmlActivityItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlParameterSet(void * id, const QCString & s) : UmlBaseParameterSet(id, s) {
    }

    //write the definition if it is not empty for the target language
    //does nothing at this level, parameterset is defined at activity level in the norme
    virtual void write(FileOut & out);

    //goes among tree to memorize incoming flow
    //does nothing at this level
    
    virtual void memo_incoming_flow();

};

#endif
