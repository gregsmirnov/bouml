
#include "UmlRelation.h"

#include "UmlClass.h"
#include "UmlAttribute.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "PythonSettings.h"

QCString UmlRelation::sKind() {
  return "relation";
}

void UmlRelation::memo_ref() {
  switch (relationKind()) {
  case aGeneralisation:
  case aRealization:
  case aDependency:
    return;
  default:
    if (visibility() == PublicVisibility)
      UmlAttribute::attrs.addElement(this);
    UmlItem::memo_ref();
    break;
  }
}

void UmlRelation::html(QCString, unsigned int, unsigned int) {
  bool extension;
  
  switch (relationKind()) {
  case aGeneralisation:
  case aRealization:
  case aDependency:
    return;
  case aDirectionalAssociation:
    extension = (parent()->stereotype() == "stereotype") &&
      (roleType()->stereotype() == "metaclass");
    break;
  default:
    extension = FALSE;
  }

  define();

  if (extension)
    fw.write("<table><tr><td><div class=\"element\">Extension</div></td></tr></table>");
  else {
    fw.write("<table><tr><td><div class=\"element\">Relation <b>");
    writeq(name());
    fw.write("</b></div></td></tr></table>");
  }

  QCString s = description();

  if (!s.isEmpty()) {
    fw.write("<p>");
    if (! javaDecl().isEmpty())
      gen_java_decl(s, TRUE);
    else if (! phpDecl().isEmpty())
      gen_php_decl(s, TRUE);
    else if (! pythonDecl().isEmpty())
      gen_python_decl(s);
    else
      gen_cpp_decl(s, TRUE);
    fw.write("<br /></p>");
  }

  if (extension) {
    fw.write("<p>Extend ");
    roleType()->write();
    fw.write("</p>");
  }
  else {
    fw.write("<p>Declaration :</p><ul>");
    
    fw.write("<li>Uml : ");
    gen_uml_decl();
    fw.write("</li>");
    
    s = cppDecl();
    
    if (!s.isEmpty()) {
      fw.write("<li>C++ : ");
      gen_cpp_decl(s, FALSE);
      fw.write("</li>");
    }
    
    s = javaDecl();
    
    if (!s.isEmpty()) {
      fw.write("<li>Java : ");
      gen_java_decl(s, FALSE);
      fw.write("</li>");
    }
    
    s = phpDecl();
    
    if (!s.isEmpty()) {
      fw.write("<li>Php : ");
      gen_php_decl(s, FALSE);
      fw.write("</li>");
    }
    
    s = pythonDecl();
    
    if (!s.isEmpty()) {
      fw.write("<li>Python : ");
      gen_python_decl(s);
      fw.write("</li>");
    }
    
    fw.write("</ul>");
  }

  annotation_constraint();
  write_properties();
 
  unload(FALSE, FALSE);
}

void UmlRelation::gen_cpp_decl(QCString s, bool descr) {
  const char * p;

  if (! descr) {
    write((cppVisibility() == DefaultVisibility)
	  ? visibility() : cppVisibility(),
	  cppLanguage);
    fw.write(": ");
    p = bypass_comment(s);
  }
  else
    p = s;
  
  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (isClassMember())
	fw.write("static ");
    }
    else if (!strncmp(p, "${const}", 8)) {
      p += 8;
      if (isReadOnly())
	fw.write("const ");
    }
    else if (!strncmp(p, "${mutable}", 10)) {
      p += 10;
      if (isCppMutable())
	fw.write("mutable ");
    }
    else if (!strncmp(p, "${volatile}", 11)) {
      p += 11;
      if (isVolatile())
	fw.write("volatile ");
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      roleType()->write();
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(roleName());
    }
    else if (!strncmp(p, "${multiplicity}", 15)) {
      p += 15;
      
      QCString m = multiplicity();

      if (m.isEmpty() || (((const char *) m)[0] != '[')) {
	fw.write("[");
	writeq(m);
	fw.write("]");
      }
      else
	writeq(m);
    }
    else if (!strncmp(p, "${stereotype}", 13)) {
      p += 13;
      writeq(CppSettings::relationAttributeStereotype(stereotype()));
    }
    else if (!strncmp(p, "${value}", 8) || !strncmp(p, "${h_value}", 10)) {
      break;
    }
    else if (!strncmp(p, "${association}", 14)) {
      p += 14;
      write(association(), cppLanguage);
    }
    else if (*p == '\r')
      p += 1;
    else if (*p == '\n') {
      if (descr) {
	fw.write("<br />");
	p += 1;
      }
      else {
	fw.write(' ');
	
	do
	  p += 1;
	while ((*p != 0) && (*p <= ' '));
      }
    }
    else if (*p == ';') {
      if (descr)
	fw.write(*p++);
      else
	break;
    }
    else if (*p == '@')
      manage_alias(p);
    else
      writeq(*p++);
  }
}

void UmlRelation::gen_java_decl(QCString s, bool descr) {
  const char * p = bypass_comment(s);

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      write(visibility(), javaLanguage);
      fw.write(' ');
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (isClassMember())
	fw.write("static ");
    }
    else if (!strncmp(p, "${transient}", 12)) {
      p += 12;
      if (isJavaTransient())
	fw.write("transient ");
    }
    else if (!strncmp(p, "${volatile}", 11)) {
      p += 11;
      if (isVolatile())
	fw.write("volatile ");
    }
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (isReadOnly())
	fw.write("final ");
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      roleType()->write();
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(roleName());
    }
    else if (!strncmp(p, "${multiplicity}", 15)) {
      p += 15;
      
      QCString m = multiplicity();

      if (! m.isEmpty()) {
	const char * s = m;
	
	if (*s != '[')
	  fw.write("[]");
	else {
	  while (*s) {
	    switch (*s++) {
	    case '[':
	      fw.write('[');
	      break;
	    case ']':
	      fw.write(']');
	    default:
	      break;
	    }
	  }
	}
      }
    }
    else if (!strncmp(p, "${stereotype}", 13)) {
      p += 13;
      writeq(JavaSettings::relationAttributeStereotype(stereotype()));
    }
    else if (!strncmp(p, "${value}", 8)) {
      p += 8;
    }
    else if (!strncmp(p, "${association}", 14)) {
      p += 14;
      write(association(), javaLanguage);
    }
    else if (!strncmp(p, "${@}", 4))
      p += 4;
    else if (*p == '\r')
      p += 1;
    else if (*p == '\n') {
      if (descr) {
	fw.write("<br />");
	p += 1;
      }
      else {
	fw.write(' ');
	
	do
	  p += 1;
	while ((*p != 0) && (*p <= ' '));
      }
    }
    else if (*p == ';') {
      if (descr)
	fw.write(*p++);
      else
	break;
    }
    else if (*p == '@')
      manage_alias(p);
    else
      writeq(*p++);
  }
}

void UmlRelation::gen_php_decl(QCString s, bool descr) {
  const char * p = bypass_comment(s);

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      write(visibility(), phpLanguage);
      fw.write(' ');
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (isClassMember())
	fw.write("static ");
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      if (!isReadOnly())
	fw.write("$");
      writeq(roleName());
    }
    else if (!strncmp(p, "${var}", 6)) {
      p += 6;
      if (!isReadOnly() &&
	  !isClassMember() &&
	  (visibility() == PackageVisibility))
	fw.write("var ");
    }
    else if (!strncmp(p, "${value}", 8)) {
      p += 8;
    }
    else if (!strncmp(p, "${const}", 8)) {
      p += 8;
      if (isReadOnly())
	fw.write("const ");
    }
    else if (*p == '\r')
      p += 1;
    else if (*p == '\n') {
      if (descr) {
	fw.write("<br />");
	p += 1;
      }
      else {
	fw.write(' ');
	
	do
	  p += 1;
	while ((*p != 0) && (*p <= ' '));
      }
    }
    else if (*p == ';') {
      if (descr)
	fw.write(*p++);
      else
	break;
    }
    else if (*p == '@')
      manage_alias(p);
    else
      writeq(*p++);
  }
}

void UmlRelation::gen_python_decl(QCString s) {
  QCString st = PythonSettings::classStereotype(stereotype());
  const char * p = bypass_comment(s);

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(roleName());
    }
    else if (!strncmp(p, "${value}", 8)) {
      p += 8;
      writeq((defaultValue().isEmpty()) ? "None" : defaultValue());
    }
    else if (!strncmp(p, "${self}", 7)) {
      p += 7;
      if (!isClassMember())
	fw.write("self.");
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      roleType()->write();
    }
    else if (!strncmp(p, "${stereotype}", 13)) {
      p += 13;
      writeq(st);
    }
    else if (*p == '\r')
      p += 1;
    else if (*p == '@')
      manage_alias(p);
    else
      writeq(*p++);
  }
}

void UmlRelation::gen_uml_decl() {
  if (isClassMember())
    fw.write("static, ");
  write(visibility());
  writeq(roleName());
  fw.write(" : ");
  roleType()->write();

  QCString s;
  
  s = defaultValue();
  if (!s.isEmpty()) {
    if (s[0] != '=')
      fw.write(" = ");
    writeq(s);
  }

  s = multiplicity();
  if (!s.isEmpty()) {
    fw.write(", multiplicity : ");
    writeq(s);
  }

  if (isDerived())
    fw.write((isDerivedUnion()) ? ", derived union" : ", derived");
    
  if (isReadOnly())
    fw.write(", read only");
    
  if (isOrdered())
    fw.write(", ordered");
    
  if (isUnique())
    fw.write(", unique");

}

QCString UmlRelation::pretty_name() {
  return roleName();
}

