#ifndef _UMLCOMPONENT_H
#define _UMLCOMPONENT_H


#include <qcstring.h>
#include "UmlBaseComponent.h"

// This class manages 'components'
//
// You can modify it as you want (except the constructor)
class UmlComponent : public UmlBaseComponent {
  public:
    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    virtual QCString sKind();

    UmlComponent(void * id, const QCString & n) : UmlBaseComponent(id, n) {};

};

#endif
