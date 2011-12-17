#ifndef _UMLNODE_H
#define _UMLNODE_H


#include "UmlBaseNode.h"
#include <qcstring.h>

// This class manages 'nodes'
//
// You can modify it as you want (except the constructor)
class UmlNode : public UmlBaseNode {
  public:
    UmlNode(void * id, const QCString & n) : UmlBaseNode(id, n) {};

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

};

#endif
