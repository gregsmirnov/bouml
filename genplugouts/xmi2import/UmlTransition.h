#ifndef _UMLTRANSITION_H
#define _UMLTRANSITION_H


#include "UmlBaseTransition.h"
#include "UmlStateItem.h"
#include <qcstring.h>
#include <qvaluelist.h>

class FileIn;
class Token;
class UmlItem;

class UmlTransition : public UmlBaseTransition, public UmlStateItem {
  public:
     UmlTransition(void * id, const QCString & n) : UmlBaseTransition(id, n) {
    }

    static void init();

    static void solveThem();

    //import the transition starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    struct Transition {
        QCString id;

        QCString name;

        QCString source;

        QCString target;

        QCString guard;

        QCString effect;

        QCString trigger;

        QCString triggerRef;

        QCString kind;

    };
    
    

  protected:
    static QValueList<Transition> All;

};

#endif
