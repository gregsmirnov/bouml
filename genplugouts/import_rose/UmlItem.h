#ifndef _UMLITEM_H
#define _UMLITEM_H


#include "UmlBaseItem.h"
#include <qcstring.h>
#include <qdict.h>
#include "anItemKind.h"
#include <qasciidict.h>

// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n) : UmlBaseItem(id, n) {};

    virtual ~UmlItem();

    virtual void roseImport();

    virtual QCString fullName();

    void setProperties(QDict<QCString> & d);

    static void newItem(UmlItem * x, const char * id);

    static UmlItem * findItem(const char * id, anItemKind k);

    static void statistic();

    static bool scanning;


  protected:
    static QAsciiDict<UmlItem> all_items[aPackage + 1];

    static int cpt[128];

};

#endif
