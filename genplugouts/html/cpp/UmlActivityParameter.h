#ifndef _UMLACTIVITYPARAMETER_H
#define _UMLACTIVITYPARAMETER_H


#include "UmlBaseActivityParameter.h"
#include <qcstring.h>

class UmlActivityParameter : public UmlBaseActivityParameter {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityParameter(void * id, const QCString & s) : UmlBaseActivityParameter(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

};

#endif
