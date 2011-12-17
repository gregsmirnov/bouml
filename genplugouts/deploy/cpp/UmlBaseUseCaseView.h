#ifndef _UMLBASEUSECASEVIEW_H
#define _UMLBASEUSECASEVIEW_H


#include "UmlView.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlUseCaseView;
class UmlItem;

//  Manage the use case views
class UmlBaseUseCaseView : public UmlView {
  public:
    // returns a new use case view named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlUseCaseView * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseUseCaseView(void * id, const QCString & n) : UmlView(id, n) {};

};

#endif
