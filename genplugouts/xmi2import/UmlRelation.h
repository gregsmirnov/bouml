#ifndef _UMLRELATION_H
#define _UMLRELATION_H


#include "UmlBaseRelation.h"
#include <qcstring.h>

class FileIn;
class Token;
class UmlItem;

// This class manages 'relations' between classes, notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlRelation : public UmlBaseRelation {
  public:
    UmlRelation(void * id, const QCString & n) : UmlBaseRelation(id, n) {};

    //import the relation (as attribute) starting by 'tk' inside 'where'
    static void importAsAttribute(FileIn & in, Token & token, UmlItem * where);

    static void init();

    //import the relation starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

};

#endif
