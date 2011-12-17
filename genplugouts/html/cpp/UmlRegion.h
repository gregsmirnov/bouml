#ifndef _UMLREGION_H
#define _UMLREGION_H


#include <qcstring.h>
#include "UmlBaseRegion.h"
#include "UmlStateItem.h"

class UmlRegion : public UmlBaseRegion, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRegion(void * id, const QCString & s) : UmlBaseRegion(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    virtual bool chapterp();

};

#endif
