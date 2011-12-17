#ifndef _UMLACTIVITYPIN_H
#define _UMLACTIVITYPIN_H


#include "UmlBaseActivityPin.h"
#include <qcstring.h>

class UmlActivityPin : public UmlBaseActivityPin {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityPin(void * id, const QCString & s) : UmlBaseActivityPin(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

};

#endif
