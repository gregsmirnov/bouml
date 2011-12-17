
#include "UmlAttribute.h"

#include "CppSettings.h"
#include "JavaSettings.h"
#include "IdlSettings.h"
#include "PhpSettings.h"
#include "PythonSettings.h"
void UmlAttribute::uml2cpp(bool) {
  QCString st = CppSettings::classStereotype(parent()->stereotype());
  
  if ((st == "typedef") || (st == "ignored"))
    set_CppDecl("");
  else if (st == "enum")
    set_CppDecl(CppSettings::enumItemDecl());
  else if (st == "ignored")
    set_CppDecl("");
  else
    set_CppDecl(CppSettings::attributeDecl(multiplicity()));
}

void UmlAttribute::uml2java(bool) {
  QCString st = JavaSettings::classStereotype(parent()->stereotype());
  
  if (st == "enum_pattern")
    set_JavaDecl(JavaSettings::enumPatternItemDecl());
  else if (st == "enum")
    set_JavaDecl(JavaSettings::enumItemDecl());
  else if (st == "ignored")
    set_JavaDecl("");
  else
    set_JavaDecl(JavaSettings::attributeDecl(multiplicity()));
}

void UmlAttribute::uml2idl(bool) {
  QCString st = IdlSettings::classStereotype(parent()->stereotype());
  
  if (st == "enum")
    set_IdlDecl(IdlSettings::enumItemDecl());
  else if (st == "union")
    set_IdlDecl(IdlSettings::unionItemDecl(multiplicity()));
  else if (isReadOnly() && !defaultValue().isEmpty())
    set_IdlDecl(IdlSettings::constDecl(multiplicity()));
  else if (st == "ignored")
    set_IdlDecl("");
  else
    set_IdlDecl(IdlSettings::attributeDecl(multiplicity()));
}

void UmlAttribute::uml2php(bool) {
  QCString st = PhpSettings::classStereotype(parent()->stereotype());
  
  if (st == "enum")
    set_PhpDecl(PhpSettings::enumItemDecl());
  else if (st == "ignored")
    set_PhpDecl("");
  else
    set_PhpDecl(PhpSettings::attributeDecl());
}

void UmlAttribute::uml2python(bool) {
  QCString st = PythonSettings::classStereotype(parent()->stereotype());
  
  if (st == "enum")
    set_PythonDecl(PythonSettings::enumItemDecl());
  else if (st == "ignored")
    set_PythonDecl("");
  else
    set_PythonDecl(PythonSettings::attributeDecl(multiplicity()));
}

