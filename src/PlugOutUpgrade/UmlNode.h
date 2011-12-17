#ifndef UMLNODE_H
#define UMLNODE_H

#include "UmlBaseNode.h"

// This class manages 'nodes'
// You can modify it as you want (except the constructor)

class UmlNode : public UmlBaseNode {
  public:
    UmlNode(void * id, const QCString & n)
      : UmlBaseNode(id, n) {};
};

#endif
