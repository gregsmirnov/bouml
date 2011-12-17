#ifndef _UMLACTIVITYCONTROLNODE_H
#define _UMLACTIVITYCONTROLNODE_H


#include "UmlBaseActivityControlNode.h"
#include <qcstring.h>

class UmlActivityControlNode : public UmlBaseActivityControlNode {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityControlNode(void * id, const QCString & s) : UmlBaseActivityControlNode(id, s) {
    }

};

#endif
