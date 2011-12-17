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

    virtual int orderWeight();

};

#endif
