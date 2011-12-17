#ifndef _UMLREGION_H
#define _UMLREGION_H


#include "UmlBaseRegion.h"
#include "UmlStateItem.h"
#include <qcstring.h>

class UmlClass;
class UmlState;

class UmlRegion : public UmlBaseRegion, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRegion(void * id, const QCString & s) : UmlBaseRegion(id, s) {
    }

    //  initialization before generation
    
    virtual void init(UmlClass * mother, QCString path, QCString pretty_path, UmlState * state);

};

#endif
