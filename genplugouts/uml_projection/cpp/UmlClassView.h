#ifndef _UMLCLASSVIEW_H
#define _UMLCLASSVIEW_H


#include "UmlBaseClassView.h"
#include <qcstring.h>

class UmlDeploymentView;

// This class manages 'class view'
//
// You can modify it as you want (except the constructor)
class UmlClassView : public UmlBaseClassView {
  public:
    UmlClassView(void * id, const QCString & n) : UmlBaseClassView(id, n) {};

    virtual void uml2cpp(bool rec);

    virtual void uml2java(bool rec);

    virtual void uml2idl(bool rec);

    virtual void uml2php(bool rec);

    virtual void uml2python(bool rec);

    UmlDeploymentView * deploymentView();

};

#endif
