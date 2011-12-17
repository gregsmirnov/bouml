
#include "UmlClassView.h"

#include "UmlCom.h"
#include "UmlDeploymentView.h"
void UmlClassView::uml2cpp(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2cpp(rec);
  }
}

void UmlClassView::uml2java(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2java(rec);
  }
}

void UmlClassView::uml2idl(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2idl(rec);
  }
}

void UmlClassView::uml2php(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2php(rec);
  }
}

void UmlClassView::uml2python(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2python(rec);
  }
}

UmlDeploymentView * UmlClassView::deploymentView() {
  UmlDeploymentView * depl = associatedDeploymentView();
  
  if (depl == 0) {
    depl = UmlDeploymentView::create((UmlPackage *) parent(), name());
    
    if (depl == 0) {
      UmlCom::trace("<b>cannot create deployment view '" +
		    name() + "' in package '" + parent()->name() + "'</b><br>");
      throw 0;
    }
    
    set_AssociatedDeploymentView(depl);
  }

  return depl;
}

