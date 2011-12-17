#ifndef _UMLITEM_H
#define _UMLITEM_H


#include "UmlBaseItem.h"
#include <qcstring.h>

#include <qdict.h>
// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QCString & n) : UmlBaseItem(id, n) {};

    virtual ~UmlItem();

    //Entry point, wrong plug-out usage in this case
    
    virtual void fileControl(bool ci);

    //Put files list in 'files', without considering 'rec' here contrarilly to the package case
    virtual void getFiles(QDict<void> & files, unsigned rec);

};

#endif
