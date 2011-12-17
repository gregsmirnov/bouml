#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "UmlBasePackage.h"
#include <qcstring.h>
#include <qlist.h>

#include "UmlDeploymentView.h"
class UmlArtifact;
class UmlDeploymentView;
class File;

// This class manages a 'package', notes that the project itself is a package
//
// You can modify it as you want (except the constructor)
class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const QCString & n) : UmlBasePackage(id, n) {
      cpt[kind()] += 1;
    };

    virtual void roseImport();

    virtual QCString fullName();

    void subArtifacts(QList<UmlArtifact> & l, QCString name, QCString deplview_name);

    void subDeplViews(QList<UmlDeploymentView> & l, QCString s);

    UmlPackage * addPackage(QCString s);

    UmlDeploymentView * addDeplView(QCString s, UmlPackage * p);


  private:
    bool import(File & f);

    void readUseCaseView(File & f);

    void readClassView(File & f);

    void readComponentView(File & f);

    void readDeploymentView(File & f);


  protected:
    QList<UmlDeploymentView> _deplviews;

};

#endif
