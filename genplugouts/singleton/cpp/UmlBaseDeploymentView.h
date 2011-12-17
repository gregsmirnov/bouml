#ifndef _UMLBASEDEPLOYMENTVIEW_H
#define _UMLBASEDEPLOYMENTVIEW_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlDeploymentView;
class UmlPackage;

//  Manage the deployment views
class UmlBaseDeploymentView : public UmlItem {
  public:
    // returns a new deployment view named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlDeploymentView * create(UmlPackage * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseDeploymentView(void * id, const QCString & n) : UmlItem(id, n) {};

};

#endif
