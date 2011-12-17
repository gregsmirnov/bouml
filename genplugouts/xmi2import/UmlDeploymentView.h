#ifndef _UMLDEPLOYMENTVIEW_H
#define _UMLDEPLOYMENTVIEW_H


#include "UmlBaseDeploymentView.h"
#include <qcstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

// This class manages 'deployment view'
//
// You can modify it as you want (except the constructor)
class UmlDeploymentView : public UmlBaseDeploymentView {
  public:
    UmlDeploymentView(void * id, const QCString & n) : UmlBaseDeploymentView(id, n) {};

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

};

#endif
