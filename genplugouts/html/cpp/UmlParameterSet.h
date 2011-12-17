#ifndef _UMLPARAMETERSET_H
#define _UMLPARAMETERSET_H


#include "UmlBaseParameterSet.h"
#include <qcstring.h>
#include "UmlActivityItem.h"

class UmlParameterSet : public UmlBaseParameterSet, public UmlActivityItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlParameterSet(void * id, const QCString & s) : UmlBaseParameterSet(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

};

#endif
