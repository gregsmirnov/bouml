
#include "UmlPackage.h"
#include "UmlArtifact.h"

#include <qfiledialog.h>

#include "File.h"
#include "UmlCom.h"
#include "UmlClassView.h"
#include "UmlUseCaseView.h"
#include "UmlDeploymentView.h"
#include "Artifact.h"

void UmlPackage::roseImport() {
  QString s = QFileDialog::getOpenFileName(QString::null, "*.mdl", 0);

  if (!s.isEmpty()) {
    File f(s, "");

    if (! f.open(IO_ReadOnly)) {
      UmlCom::trace("<br>cannot open " + s);
      throw 0;
    }
    
    UmlItem * prj = this;
    
    while (prj->parent() != 0)
      prj = prj->parent();
    
    prj->set_childrenVisible(FALSE);
    
    try {
      UmlCom::message("Phase 1");
      scanning = TRUE;
      
      while (import(f))
	;
      
      scanning = FALSE;
      UmlCom::message("Phase 2");
      f.rewind();
      
      while (import(f))
	;
    }
    catch (...) {
      set_childrenVisible(TRUE);
      throw 0;
    }
    
    set_childrenVisible(TRUE);
  }
}

QCString UmlPackage::fullName() {
  return (this == getProject()) ? name() : UmlItem::fullName();
}

void UmlPackage::subArtifacts(QList<UmlArtifact> & l, QCString name, QCString deplview_name) {
  UmlDeploymentView * deplview;
  
  for (deplview = _deplviews.first(); deplview != 0; deplview = _deplviews.next()) {
    if (deplview->baseName() == deplview_name) {
      const QVector<UmlItem> ch = deplview->children();
      unsigned i;
      
      for (i = 0; i != ch.size(); i += 1) {
	UmlItem * it = ch[i];
	
	if ((it->kind() == anArtifact) && (it->name() == name))
	  l.append((UmlArtifact *) it);
      }
    }
  }

}

void UmlPackage::subDeplViews(QList<UmlDeploymentView> & l, QCString s) {
  UmlDeploymentView * deplview;
  
  for (deplview = _deplviews.first(); deplview != 0; deplview = _deplviews.next())
    if (deplview->baseName() == s)
      l.append(deplview);
}

UmlPackage * UmlPackage::addPackage(QCString s) {
  UmlPackage * r;
  
  while ((r = UmlPackage::create(this, s)) == 0)
    s += "_";
    
  return r;
}

UmlDeploymentView * UmlPackage::addDeplView(QCString s, UmlPackage * p) {
  UmlDeploymentView * dpv;
  QCString bn = s;
  
  while ((dpv = UmlDeploymentView::create(p, s, bn)) == 0)
    s += '_';
    
  _deplviews.append(dpv);
  
  return dpv;
}

bool UmlPackage::import(File & f) {
  QCString s;

  switch (f.read(s)) {
  case -1:
    return FALSE;
  case '(':
    break;
  default:
    f.syntaxError(s);
  }

  if ((f.read(s) != ATOM) || (s != "object"))
    f.syntaxError(s, "object");

  if (f.read(s) != ATOM)
    f.syntaxError(s);

  if (s == "Design") {
    f.read(s);	// "Logical View"

    for (;;) {
      switch (f.read(s)) {
      case ATOM:
        break;
      case ')':
        return TRUE;
      case -1:
        f.eof();
        throw 0;
      default:
        f.syntaxError(s, "an atom");
      }

      if (s == "root_usecase_package")
        readUseCaseView(f);
      else if (s == "root_category")
        readClassView(f);
      else if (s == "root_subsystem")
        readComponentView(f);
      else if (s == "process_structure")
        readDeploymentView(f);
      else {
#ifdef DEBUGROSE
        if (s != "properties")
	  UmlCom::trace("<br>import does not manage '" + s + "'");
#endif
        f.skipNextForm();
      }
    }
    return TRUE;
  }
  else if (s == "Petal") {
    f.skipBlock();
    return TRUE;
  }
  else {
    UmlCom::trace("<br>" + s + " : not yet managed " + f.context());
    throw 0;
  }
}

void UmlPackage::readUseCaseView(File & f) {
  f.read("(");
  f.read("object");
  f.read("Class_Category");

  UmlUseCaseView::import(this, f);
}

void UmlPackage::readClassView(File & f) {
  f.read("(");
  f.read("object");
  f.read("Class_Category");

  UmlClassView::import(this, f);
}

void UmlPackage::readComponentView(File & f) {
  if (scanning) {
    f.read("(");
    f.read("object");
    f.read("SubSystem");
    Artifact::import_component_view(f);
  }
  else
    f.skipNextForm();
}

void UmlPackage::readDeploymentView(File & f) {
  f.read("(");
  f.read("object");
  f.read("Processes");

  UmlDeploymentView::import(this, f);
}

