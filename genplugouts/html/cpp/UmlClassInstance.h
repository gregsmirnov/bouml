#ifndef _UMLCLASSINSTANCE_H
#define _UMLCLASSINSTANCE_H


#include "UmlBaseClassInstance.h"
#include <qcstring.h>

class UmlClassInstance : public UmlBaseClassInstance {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlClassInstance(void * id, const QCString & s) : UmlBaseClassInstance(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

};

#endif
