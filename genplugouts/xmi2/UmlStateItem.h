#ifndef _UMLSTATEITEM_H
#define _UMLSTATEITEM_H


class UmlTransition;

class UmlStateItem {
  public:
    //memorize trans starting in sub elements to produce them later
    //does nothing at this level
    virtual void memo_trans(UmlTransition * tr);

    //goes among tree to memorize incoming transitions
    //does nothing at this level
    virtual void memo_incoming_trans();

    //memorize a new incoming transition
    //does nothing at this level
    virtual void add_incoming_trans(UmlTransition * tr);

    //just to not have warning at compile time
    virtual ~UmlStateItem();

};

#endif
