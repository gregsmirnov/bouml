#ifndef _UMLEXPANSIONREGION_H
#define _UMLEXPANSIONREGION_H


#include "UmlBaseExpansionRegion.h"
#include <qcstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

class UmlExpansionRegion : public UmlBaseExpansionRegion {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlExpansionRegion(void * id, const QCString & s) : UmlBaseExpansionRegion(id, s) {
    }

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    static void init();

    //import the activity region 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

};

#endif
