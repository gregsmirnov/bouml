#ifndef _UMLREGION_H
#define _UMLREGION_H


#include "UmlBaseRegion.h"
#include "UmlStateItem.h"
#include <qcstring.h>
#include <qlist.h>

class UmlTransition;
class FileOut;

class UmlRegion : public UmlBaseRegion, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRegion(void * id, const QCString & s) : UmlBaseRegion(id, s) {
    }

    //goes among tree to memorize incoming transitions
    
    virtual void memo_incoming_trans();

    //to ask parent to memorize trans starting in sub elements to produce them later
    //Don't memorize transitions in the region itself to bypass Eclipse's bug
    
    virtual void memo_trans(UmlTransition * tr);

    virtual void write(FileOut & out);


  protected:
#if 0
    // To bypass Eclipse's bug
    QList<UmlTransition> _trans;
#endif

};

#endif
