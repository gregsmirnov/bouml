#ifndef _UMLATTRIBUTE_H
#define _UMLATTRIBUTE_H


#include "UmlBaseAttribute.h"
#include <qcstring.h>

class FileIn;
class Token;
class UmlItem;

// This class manages 'attribute', notes that the class 'UmlClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)
class UmlAttribute : public UmlBaseAttribute {
  public:
    UmlAttribute(void * id, const QCString & n) :  UmlBaseAttribute(id, n) {};

    static void init();

    //import the attribute starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    //  call at end of import : try to solve type
    virtual void solve(QCString idref);


  protected:
    //import the lower multiplicity starting by 'tk' inside 'where'
    void importMultiplicity(FileIn & in, Token & token, bool upper);

};

#endif
