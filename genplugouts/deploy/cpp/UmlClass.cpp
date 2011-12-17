
#include "UmlClass.h"
#include "UmlDeploymentView.h"

#include "UmlArtifact.h"
#include "UmlCom.h"
#include "aLanguage.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "IdlSettings.h"
#include "PhpSettings.h"
#include "PythonSettings.h"
void UmlClass::deploy(UmlDeploymentView * view) {
  int flags = deployable();
  
  if (flags == 0)
    return;
      
  UmlArtifact * a = associatedArtifact();
  
  if (a == 0) {
    UmlCom::trace("Create <i>artifact</i> " + name());
    
    a = UmlArtifact::create(view, name());
      
    if (a == 0) {
      UmlCom::trace("cannot create <i>artifact</i> named " + name());
      throw 0;
    }
  
    a->set_Stereotype("source");
    a->addAssociatedClass(this);
    a->set_CppHeader("");
    a->set_CppSource("");
    a->set_JavaSource("");
    a->set_IdlSource("");
    a->set_PhpSource("");
    a->set_PythonSource("");
  }
  
  if ((flags & (1 << cppLanguage)) != 0) {
    a->set_CppHeader(CppSettings::headerContent());
    a->set_CppSource(CppSettings::sourceContent());
  }
  
  if ((flags & (1 << javaLanguage)) != 0)
    a->set_JavaSource(JavaSettings::sourceContent());
  
  if ((flags & (1 << idlLanguage)) != 0)
    a->set_IdlSource(IdlSettings::sourceContent());
  
  if ((flags & (1 << phpLanguage)) != 0)
    a->set_PhpSource(PhpSettings::sourceContent());
  
  if ((flags & (1 << pythonLanguage)) != 0)
    a->set_PythonSource(PythonSettings::sourceContent());
}

int UmlClass::deployable() {
  QCString st = stereotype();
  
  if ((st == "metaclass") || (st == "stereotype"))
    return 0;
  
  int r = 0;
  
  if (!isCppExternal() && !cppDecl().isEmpty())
    r += (1 << cppLanguage);
  
  if (!isJavaExternal() && !javaDecl().isEmpty())
    r += (1 << javaLanguage);
  
  if (!isIdlExternal() && !idlDecl().isEmpty())
    r += (1 << idlLanguage);
  
  if (!isPhpExternal() && !phpDecl().isEmpty())
    r += (1 << phpLanguage);
  
  if (!isPythonExternal() && !pythonDecl().isEmpty())
    r += (1 << pythonLanguage);
  
  return r;
}

