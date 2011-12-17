
#include "UmlAttribute.h"
#include "File.h"
#include "UmlClass.h"

#include "UmlCom.h"
#include "util.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "IdlSettings.h"
void UmlAttribute::import(File & f) {
  if (scanning) {
    f.skipBlock();
    return;
  }

  QCString s;
  UmlTypeSpec t;
        
  for (;;) {
    switch (f.read(s)) {
    case ')':
      set_Type(t);
      return;
    case ATOM:
      break;
    default:
      f.syntaxError(s);
    }
    
    // atom
    if (s == "type") {
      if (f.read(s) != STRING)
	f.syntaxError(s, "attribute type");
      t.explicit_type = s;
    }
    else if (s == "initv") {
      if (f.read(s) != STRING)
	f.syntaxError(s, "attribute initial value");
      s = "= " + s;
      set_DefaultValue(s);
    }
    else if (s == "static")
      set_isClassMember(f.readBool());
    else if (s == "quidu") {
      if (f.read(s) != STRING)
	f.syntaxError(s, "wrong quidu");
      if (UmlClass::replaceType(t, s, "${type}")) {
	switch (((UmlClass *) parent())->language()) {
	case Cplusplus:
	case AnsiCplusplus:
	case VCplusplus:
	  set_CppDecl(replace(cppDecl(), "${type}", s));
	  break;
	case Oracle8:
	  break;
	case Corba:
	  set_IdlDecl(replace(idlDecl(), "${type}", s));
	  break;
	case Java:
	  set_JavaDecl(replace(javaDecl(), "${type}", s));
	  break;
	default:
	  break;
	}
      }
    }
    else if (s == "exportControl")
      set_Visibility(f.readVisibility());
    else
      f.skipNextForm();
  }
}

void UmlAttribute::import(File & f, UmlClass * parent)
{
  QCString s;

  if (f.read(s) != STRING)
    f.syntaxError(s, "attributes's name");
    
  QCString id;
  QCString ste;
  QCString doc;
  QDict<QCString> prop;
  QCString s2;
  int k;
  
  do {
    k = f.readDefinitionBeginning(s2, id, ste, doc, prop);
  } while (id.isEmpty());

  UmlAttribute * x;

  if (scanning) {
    if ((x = UmlBaseAttribute::create(parent, legalName(s))) == 0) {
      UmlCom::trace("<br>cannot create attribute '" + s + "' in " +
		    parent->fullName());
      throw 0;
    }
    newItem(x, id);

    if (!ste.isEmpty())
      x->set_Stereotype(ste);

    if (!doc.isEmpty())
      x->set_Description(doc);
  }
  else if ((x = (UmlAttribute *) findItem(id, anAttribute)) == 0) {
    UmlCom::trace("<br>unknown attribute '" + s + "' in " +
		  parent->fullName());
    throw 0;
  }
  else {
    switch (((UmlClass *) x->parent())->language()) {
    case Cplusplus:
    case AnsiCplusplus:
    case VCplusplus:
      x->cplusplus(prop);
      break;
    case Oracle8:
      x->oracle8(prop);
      break;
    case Corba:
      x->corba(prop);
      break;
    case Java:
      x->java(prop);
      break;
    default:
      break;
    }
    
    x->setProperties(prop);
  }

  f.unread(k, s2);
  x->import(f);

}

void UmlAttribute::importIdlConstant(UmlClass * parent, const QCString & id, const QCString & s, const QCString & doc, QDict<QCString> & prop)
{
  UmlAttribute * x;

  if ((x = UmlBaseAttribute::create(parent, legalName(s))) == 0) {
    UmlCom::trace("<br>cannot create attribute '" + s + "' in " +
		  parent->fullName());
    throw 0;
  }
  newItem(x, id);

  if (!doc.isEmpty())
    x->set_Description(doc);

  x->set_IdlDecl(IdlSettings::constDecl());
  
  QCString * v;
  
  if ((v = prop.find("CORBA/ImplementationType")) != 0) {
    if (!v->isEmpty()) {
      UmlTypeSpec t;
      
      t.explicit_type = *v;
      x->set_Type(t);
    }
    prop.remove("CORBA/ImplementationType");
  }

  if ((v = prop.find("CORBA/ConstValue")) != 0) {
    if (!v->isEmpty()) {
      *v = "= " + *v;
      x->set_DefaultValue(*v);
    }
    prop.remove("CORBA/ConstValue");
  }

  x->setProperties(prop);
}

void UmlAttribute::cplusplus(QDict<QCString> &) {
  set_CppDecl((parent()->stereotype() == "enum")
	      ? CppSettings::enumItemDecl()
	      : CppSettings::attributeDecl());
}

void UmlAttribute::oracle8(QDict<QCString> &) {
}

void UmlAttribute::corba(QDict<QCString> & prop) {
  QCString * v;
  QCString decl;
  
  if (parent()->stereotype() == "union")
    decl = IdlSettings::unionItemDecl();
  else if (parent()->stereotype() == "enum")
    decl = IdlSettings::enumItemDecl();
  else
    decl = IdlSettings::attributeDecl();
    
  if ((v = prop.find("CORBA/IsReadOnly")) != 0) {
    if (*v == "TRUE")
      set_isReadOnly(TRUE);
    prop.remove("CORBA/IsReadOnly");
  }
  
  if ((v = prop.find("CORBA/CaseSpecifier")) != 0) {
    set_IdlCase(*v);	// !!!!!!!!!!! chercher union attr
    prop.remove("CORBA/CaseSpecifier");
  }
  
  if ((v = prop.find("CORBA/ArrayDimensions")) != 0) {
    if (!v->isEmpty()) {
      int index;
      
      if ((index = decl.find("${name}")) != -1)
	decl.insert(index + 7, "[" + *v + "]");
    }
    
    prop.remove("CORBA/ArrayDimensions");
  }

  set_IdlDecl(decl);
}

void UmlAttribute::java(QDict<QCString> & prop) {
  QCString * v;
    
  if ((v = prop.find("Java/Final")) != 0) {
    if (*v == "TRUE")
      set_isReadOnly(TRUE);
    prop.remove("Java/Final");
  }
  if ((v = prop.find("Java/Transient")) != 0) {
    if (*v == "TRUE")
      set_isJavaTransient(TRUE);
    prop.remove("Java/Transient");
  }
  if ((v = prop.find("Java/Volatile")) != 0) {
    if (*v == "TRUE")
      set_isVolatile(TRUE);
    prop.remove("Java/Volatile");
  }

  set_JavaDecl(JavaSettings::attributeDecl());
}

