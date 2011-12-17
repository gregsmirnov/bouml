#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "UmlBasePackage.h"
#include <qcstring.h>

// This class manages a 'package', notes that the project itself is a package
//
// You can modify it as you want (except the constructor)
class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const QCString & n) : UmlBasePackage(id, n) {};

    virtual void sort();

    virtual int orderWeight();

};

#endif
