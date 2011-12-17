#ifndef _UMLDEPLOYMENTVIEW_H
#define _UMLDEPLOYMENTVIEW_H


#include "UmlBaseDeploymentView.h"
#include <qcstring.h>

class File;
class UmlPackage;

// This class manages 'deployment view'
//
// You can modify it as you want (except the constructor)
class UmlDeploymentView : public UmlBaseDeploymentView {
  public:
    UmlDeploymentView(void * id, const QCString & n) : UmlBaseDeploymentView(id, n) {
      cpt[kind()] += 1;
    };

    void import(File & f);

    void readObjects(File & f);

    static UmlDeploymentView * create(UmlPackage * parent, const char * s, QCString bn);

    static void import(UmlPackage * parent, File & f);


  protected:
    QCString _base_name;


  public:
    const QCString baseName() const;

};

inline const QCString UmlDeploymentView::baseName() const {
  return _base_name;
}

#endif
