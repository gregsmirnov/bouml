#ifndef _UMLCLASSITEM_H
#define _UMLCLASSITEM_H


#include "UmlBaseClassItem.h"
#include <qcstring.h>

// This class globaly manages class's children including UmlClassExtraMember
//
// You can modify it as you want (except the constructor)
class UmlClassItem : public UmlBaseClassItem {
  public:
    UmlClassItem(void * id, const QCString & n) : UmlBaseClassItem(id, n) {};

};

#endif
