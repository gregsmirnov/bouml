#ifndef _UMLNODE_H
#define _UMLNODE_H


#include "UmlBaseNode.h"
#include <qcstring.h>

class FileIn;
class Token;
class UmlItem;

// This class manages 'nodes'
//
// You can modify it as you want (except the constructor)
class UmlNode : public UmlBaseNode {
  public:
    UmlNode(void * id, const QCString & n) : UmlBaseNode(id, n) { NumberOf += 1; };

    static void init();

    //import the node starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    static int numberOf() { return NumberOf; };


  protected:
    static int NumberOf;

};

#endif
