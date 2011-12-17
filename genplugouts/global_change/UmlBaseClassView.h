#ifndef _UMLBASECLASSVIEW_H
#define _UMLBASECLASSVIEW_H


#include "UmlView.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlClassView;
class UmlPackage;
class UmlDeploymentView;

// Manage the class views
class UmlBaseClassView : public UmlView {
  public:
    // returns a new class view named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlClassView * create(UmlPackage * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the optional associated deployment view
    UmlDeploymentView * associatedDeploymentView();

    // sets the associated deployment view, arg may be null to unset it
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    
    bool set_AssociatedDeploymentView(UmlDeploymentView * v);


  private:
    UmlDeploymentView * _assoc_view;


  protected:
    //internal, do NOT use it
    
    virtual void read_uml_();

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseClassView(void * id, const QCString & n) : UmlView(id, n) {};

};

#endif
