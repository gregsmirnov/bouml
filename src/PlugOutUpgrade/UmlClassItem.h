#ifndef UMLCLASSITEM_H
#define UMLCLASSITEM_H

#include <qlist.h>

#include "UmlBaseClassItem.h"

// This class globaly manages class's children including UmlClassExtraMember
// You can modify it as you want (except the constructor)

class UmlClassItem : public UmlBaseClassItem {
  public:
    UmlClassItem(void * id, const QCString & n)
      : UmlBaseClassItem(id, n) {};
};

#endif
