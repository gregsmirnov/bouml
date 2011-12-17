#ifndef _UMLCLASSINSTANCE_H
#define _UMLCLASSINSTANCE_H


#include "UmlBaseClassInstance.h"
#include <qcstring.h>

class FileOut;

class UmlClassInstance : public UmlBaseClassInstance {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlClassInstance(void * id, const QCString & s) : UmlBaseClassInstance(id, s) {
    }

    virtual void write(FileOut & out);

};

#endif
