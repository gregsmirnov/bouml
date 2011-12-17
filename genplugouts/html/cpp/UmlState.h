#ifndef _UMLSTATE_H
#define _UMLSTATE_H


#include "UmlBaseState.h"
#include "UmlStateItem.h"
#include <qcstring.h>
#include "Vector.h"

class UmlState : public UmlBaseState, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlState(void * id, const QCString & s) : UmlBaseState(id, s) {
    }

    //returns a string indicating the king of the element
    virtual QCString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //set the html ref
    //set the state list
    virtual void memo_ref();

    static void ref_index();

    static void generate_index();

    virtual bool chapterp();


  protected:
    static Vector states;

};

#endif
