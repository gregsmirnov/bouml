#ifndef _UMLCLASS_H
#define _UMLCLASS_H


#include "UmlBaseClass.h"
#include <qcstring.h>

// This class manages 'classes', notes that the class 'ClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)
class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n) : UmlBaseClass(id, n) {};

    //virtual operation doing 'something'
    //
    //here allows to add contructor / destructor / copy constructor / operator=
    virtual void utilities();

    //add a constructor to the class
    
    void addContructor(bool expl);

    //add a destructor to the class
    
    void addDestructor(bool virt);

    //add a copy constructor to the class
    void addCopy(bool cte);

    //add operator= to the class
    void addAssign(bool cte);

};

#endif
