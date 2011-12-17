#ifndef _UMLCLASS_H
#define _UMLCLASS_H


#include "UmlBaseClass.h"
#include <qcstring.h>

class FileOut;

// This class manages 'classes', notes that the class 'ClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)

class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n) : UmlBaseClass(id, n) {};

    virtual void ref(FileOut & out);

    virtual void write(FileOut & out);

    virtual bool write_if_needed(FileOut & out);

    virtual void write_actor(FileOut & out);

};

#endif
