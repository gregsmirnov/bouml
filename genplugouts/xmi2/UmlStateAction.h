#ifndef _UMLSTATEACTION_H
#define _UMLSTATEACTION_H


#include "UmlBaseStateAction.h"
#include "UmlStateItem.h"
#include <qlist.h>

class FileOut;
class UmlTransition;

class UmlStateAction : public UmlBaseStateAction, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlStateAction(void * id) : UmlBaseStateAction(id, "") {
    }

    //goes among tree to memorize incoming transitions
    
    virtual void memo_incoming_trans();

    virtual void write(FileOut & out);


  protected:
    QList<UmlTransition> _incoming_trans;

};

#endif
