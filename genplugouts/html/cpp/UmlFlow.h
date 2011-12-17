#ifndef _UMLFLOW_H
#define _UMLFLOW_H


#include <qcstring.h>
#include "UmlBaseFlow.h"
#include "UmlActivityItem.h"

class UmlFlow : public UmlBaseFlow, public UmlActivityItem {
  public:
    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

     UmlFlow(void * id, const QCString & n) : UmlBaseFlow(id, n) {
    }

};

#endif
