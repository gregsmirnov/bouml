#ifndef _UMLCLASS_H
#define _UMLCLASS_H


#include "UmlBaseClass.h"
#include <qcstring.h>

class UmlArtifact;

// This class manages 'classes', notes that the class 'ClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)
class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n) : UmlBaseClass(id, n) {};

    virtual void uml2cpp(bool rec);

    virtual void uml2java(bool rec);

    virtual void uml2idl(bool rec);

    virtual void uml2php(bool rec);

    virtual void uml2python(bool rec);


  private:
    UmlArtifact * artifact();

};

#endif
