#ifndef _UMLRELATION_H
#define _UMLRELATION_H


#include "UmlBaseRelation.h"
#include <qcstring.h>

// This class manages 'relations' between classes, notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlRelation : public UmlBaseRelation {
  public:
    UmlRelation(void * id, const QCString & n) : UmlBaseRelation(id, n) {};

    virtual void uml2cpp(bool rec);

    virtual void uml2java(bool rec);

    virtual void uml2idl(bool rec);

    virtual void uml2php(bool rec);

    virtual void uml2Python(bool rec);

};

#endif
