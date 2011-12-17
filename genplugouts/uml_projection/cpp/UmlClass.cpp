
#include "UmlClass.h"

#include "UmlCom.h"
#include "JavaSettings.h"
#include "CppSettings.h"
#include "IdlSettings.h"
#include "PhpSettings.h"
#include "PythonSettings.h"
#include "UmlClassView.h"
#include "UmlDeploymentView.h"
#include "UmlArtifact.h"
void UmlClass::uml2cpp(bool rec) {
  if (isCppExternal())
    set_CppDecl(CppSettings::externalClassDecl());
  else {
    QCString st = CppSettings::classStereotype(stereotype());
    UmlItem * pack = parent()->parent();
    
    while (pack->kind() != aPackage)
      pack = pack->parent();
    
    if ((st == "stereotype") ||
	(st == "metaclass") ||
	(pack->stereotype() == "profile")) {
      set_CppDecl("");
      return;
    }
    
    if (st == "enum")
      set_CppDecl(CppSettings::enumDecl());
    else if (st == "union")
      set_CppDecl(CppSettings::unionDecl());
    else if (st == "struct")
      set_CppDecl(CppSettings::structDecl());
    else if (st == "typedef")
      set_CppDecl(CppSettings::typedefDecl());
    else if (st == "ignored") {
      set_CppDecl("");
      return;
    }
    else
      set_CppDecl(CppSettings::classDecl());
    
    if (rec) {
      const QVector<UmlItem> ch = children();
      unsigned n = ch.size();
      
      for (unsigned i = 0; i != n; i += 1)
	ch[i]->uml2cpp(rec);
    }
    
    if (parent()->kind() == aClassView) {
      // not nested
      UmlArtifact * art = artifact();
			 
      art->set_CppSource(CppSettings::sourceContent());
      art->set_CppHeader(CppSettings::headerContent());
    }
  }
}

void UmlClass::uml2java(bool rec) {
  if (isJavaExternal())
    set_JavaDecl(JavaSettings::externalClassDecl());
  else {
    QCString st = JavaSettings::classStereotype(stereotype());
    UmlItem * pack = parent()->parent();
    
    while (pack->kind() != aPackage)
      pack = pack->parent();
    
    if ((st == "stereotype") ||
	(st == "metaclass") ||
	(pack->stereotype() == "profile")) {
      set_CppDecl("");
      return;
    }
    
    if (st == "enum_pattern")
      set_JavaDecl(JavaSettings::enumPatternDecl());
    else if (st == "enum")
      set_JavaDecl(JavaSettings::enumDecl());
    else if (st == "interface")
      set_JavaDecl(JavaSettings::interfaceDecl());
    else if (st == "@interface") {
      QCString s = JavaSettings::interfaceDecl();
      int index = s.find("interface");
      
      if (index != -1)
	s.insert(index, '@');
      set_JavaDecl(s);
    }
    else if (st == "ignored") {
      set_JavaDecl("");
      return;
    }
    else
      set_JavaDecl(JavaSettings::classDecl());
    
    if (rec) {
      const QVector<UmlItem> ch = children();
      unsigned n = ch.size();
      
      for (unsigned i = 0; i != n; i += 1)
	ch[i]->uml2java(rec);
    }
    
    if (parent()->kind() == aClassView)
      // not nested
      artifact()->set_JavaSource(JavaSettings::sourceContent());
  }
}

void UmlClass::uml2idl(bool rec) {
  if (isIdlExternal())
    set_IdlDecl(IdlSettings::externalClassDecl());
  else {
    QCString st = IdlSettings::classStereotype(stereotype());
    UmlItem * pack = parent()->parent();
    
    while (pack->kind() != aPackage)
      pack = pack->parent();
    
    if ((st == "stereotype") ||
	(st == "metaclass") ||
	(pack->stereotype() == "profile")) {
      set_CppDecl("");
      return;
    }
    
    if (st == "struct")
      set_IdlDecl(IdlSettings::structDecl());
    else if (st == "union")
      set_IdlDecl(IdlSettings::unionDecl());
    else if (st == "enum")
      set_IdlDecl(IdlSettings::enumDecl());
    else if (st == "exception")
      set_IdlDecl(IdlSettings::exceptionDecl());
    else if (st == "typedef")
      set_IdlDecl(IdlSettings::typedefDecl());
    else if (st == "interface")
      set_IdlDecl(IdlSettings::interfaceDecl());
    else if (st == "ignored") {
      set_IdlDecl("");
      return;
    }
    else
      set_IdlDecl(IdlSettings::valuetypeDecl());
    
    if (rec) {
      const QVector<UmlItem> ch = children();
      unsigned n = ch.size();
      
      for (unsigned i = 0; i != n; i += 1)
	ch[i]->uml2idl(rec);
    }
    
    if (parent()->kind() == aClassView)
      // not nested
      artifact()->set_IdlSource(IdlSettings::sourceContent());
  }
}

void UmlClass::uml2php(bool rec) {
  if (isPhpExternal())
    set_PhpDecl(PhpSettings::externalClassDecl());
  else {
    QCString st = PhpSettings::classStereotype(stereotype());
    UmlItem * pack = parent()->parent();
    
    while (pack->kind() != aPackage)
      pack = pack->parent();
    
    if ((st == "stereotype") ||
	(st == "metaclass") ||
	(pack->stereotype() == "profile")) {
      set_CppDecl("");
      return;
    }
    
    
    if (st == "enum")
      set_PhpDecl(PhpSettings::enumDecl());
    else if (st == "interface")
      set_PhpDecl(PhpSettings::interfaceDecl());
    else if (st == "ignored") {
      set_PhpDecl("");
      return;
    }
    else
      set_PhpDecl(PhpSettings::classDecl());
    
    if (rec) {
      const QVector<UmlItem> ch = children();
      unsigned n = ch.size();
      
      for (unsigned i = 0; i != n; i += 1)
	ch[i]->uml2php(rec);
    }
    
    if (parent()->kind() == aClassView)
      // not nested
      artifact()->set_PhpSource(PhpSettings::sourceContent());
  }
}

void UmlClass::uml2python(bool rec) {
  if (isPythonExternal())
    set_PythonDecl(PythonSettings::externalClassDecl());
  else {
    QCString st = PythonSettings::classStereotype(stereotype());
    UmlItem * pack = parent()->parent();
    
    while (pack->kind() != aPackage)
      pack = pack->parent();
    
    if ((st == "stereotype") ||
	(st == "metaclass") ||
	(pack->stereotype() == "profile")) {
      set_CppDecl("");
      return;
    }
    
    
    if (st == "enum")
      set_PythonDecl(PythonSettings::enumDecl());
    else if (st == "ignored") {
      set_PythonDecl("");
      return;
    }
    else
      set_PythonDecl(PythonSettings::classDecl());
    
    if (rec) {
      const QVector<UmlItem> ch = children();
      unsigned n = ch.size();
      
      for (unsigned i = 0; i != n; i += 1)
	ch[i]->uml2python(rec);
    }
    
    if (parent()->kind() == aClassView)
      // not nested
      artifact()->set_PythonSource(PythonSettings::sourceContent());
  }
}

UmlArtifact * UmlClass::artifact() {
  // note : class is not nested
  UmlArtifact * art = associatedArtifact();
  
  if (art == 0) {
    UmlDeploymentView * depl =	((UmlClassView *) parent())->deploymentView();
    
    art = UmlArtifact::create(depl, name());
    
    if (art == 0) {
      UmlCom::trace("<b>cannot create artifact '" +
		    name() + "' in deployment view '" + depl->name() + "'</b><br>");
      throw 0;
    }
    
    art->set_Stereotype("source");
    art->addAssociatedClass(this);
  }

  return art;
}

