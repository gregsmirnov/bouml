#ifndef _UMLACTIVITYPIN_H
#define _UMLACTIVITYPIN_H


#include "UmlBaseActivityPin.h"
#include <qcstring.h>

class FileIn;
class Token;
class UmlItem;

class UmlActivityPin : public UmlBaseActivityPin {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityPin(void * id, const QCString & s) : UmlBaseActivityPin(id, s) {
    }

    static void init();

    //import the activity pin starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

};

#endif
