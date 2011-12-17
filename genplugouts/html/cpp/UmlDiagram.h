#ifndef _UMLDIAGRAM_H
#define _UMLDIAGRAM_H


#include "UmlBaseDiagram.h"
#include <qcstring.h>

// This class allows to manage diagram
//
// You can modify it as you want (except the constructor)
class UmlDiagram : public UmlBaseDiagram {
  public:
    UmlDiagram(void * id, const QCString & n) : UmlBaseDiagram(id, n) {};

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

};

#endif
