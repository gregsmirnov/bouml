#ifndef _UMLEXPANSIONNODE_H
#define _UMLEXPANSIONNODE_H


#include "UmlBaseExpansionNode.h"
#include <qcstring.h>

class FileOut;

class UmlExpansionNode : public UmlBaseExpansionNode {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlExpansionNode(void * id, const QCString & s) : UmlBaseExpansionNode(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);

};

#endif
