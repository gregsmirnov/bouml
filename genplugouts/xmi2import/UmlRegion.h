#ifndef _UMLREGION_H
#define _UMLREGION_H


#include "UmlBaseRegion.h"
#include "UmlStateItem.h"
#include <qcstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

class UmlRegion : public UmlBaseRegion, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRegion(void * id, const QCString & s) : UmlBaseRegion(id, s) {
    }

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    static void init();

    //import the region starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

};

#endif
