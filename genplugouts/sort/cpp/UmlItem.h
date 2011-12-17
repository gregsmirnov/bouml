#ifndef _UMLITEM_H
#define _UMLITEM_H


#include "UmlBaseItem.h"
#include <qcstring.h>
#include <qvector.h>


// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n) : UmlBaseItem(id, n) {};

    virtual ~UmlItem();

    virtual void sort();

    virtual int orderWeight();

    void sortChildren();

};

class VectorOfUmlItem : public QVector<UmlItem> {
  public:
    int compareItems(QCollection::Item d1, QCollection::Item d2);

};

#endif
