#ifndef _UMLPSEUDOSTATE_H
#define _UMLPSEUDOSTATE_H


#include <qcstring.h>
#include "UmlItem.h"
#include "UmlStateItem.h"

class FileIn;
class Token;

class UmlPseudoState : public UmlStateItem, public UmlItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlPseudoState(void * id, const QCString & s) : UmlItem(id, s) {
    }

    static void init();

    //import the initial pseudo state starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    //import the initial pseudo state starting by 'tk' inside 'where'
    static void importRef(FileIn & in, Token & token, UmlItem * where);

};

#endif
