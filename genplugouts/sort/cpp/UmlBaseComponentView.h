#ifndef _UMLBASECOMPONENTVIEW_H
#define _UMLBASECOMPONENTVIEW_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlComponentView;
class UmlPackage;

//  Manage the component views
class UmlBaseComponentView : public UmlItem {
  public:
    // returns a new component view named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlComponentView * create(UmlPackage * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseComponentView(void * id, const QCString & n) : UmlItem(id, n) {};

};

#endif
