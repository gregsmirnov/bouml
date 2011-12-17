#ifndef _UMLSTATE_H
#define _UMLSTATE_H


#include "UmlBaseState.h"
#include "UmlStateItem.h"
#include <qcstring.h>
#include <qlist.h>
#include <qmap.h>

class FileOut;
class UmlTransition;

class UmlState : public UmlBaseState, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
 UmlState(void * id, const QCString & s) : UmlBaseState(id, s) {
  _all.replace(s, this);
}

    virtual void write(FileOut & out);

    //goes among tree to memorize incoming transitions
    
    virtual void memo_incoming_trans();

    //memorize trans starting in sub elements to produce them later
    
    virtual void memo_trans(UmlTransition * tr);

    //memorize a new incoming transition
    void add_incoming_trans(UmlTransition * tr);

    //Return a state from its name.
    //Doesn't manage namespace/package/module
    
    static UmlState * find(QCString s);


  protected:
    QList<UmlTransition> _incoming_trans;

    QList<UmlTransition> _trans;

    static QMap<QCString, UmlState *> _all;

};

#endif
