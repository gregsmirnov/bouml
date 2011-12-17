#ifndef _UMLCLASS_H
#define _UMLCLASS_H


#include "UmlBaseClass.h"
#include <qcstring.h>

class UmlOperation;

// This class manages 'classes', notes that the class 'ClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)
class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n) : UmlBaseClass(id, n) {};

    //  set default C++/Java definition if needed
    //  set it usefull
    void defaultDef();

    //  add a trigger, returns the one for the class
    //  Don't return on error
    UmlOperation * trigger(QCString s, UmlClass * machine, UmlClass * anystate);

    //add a virtual destructor if needed
    //initial definition is empty
    void addDestructor();

};

#endif
