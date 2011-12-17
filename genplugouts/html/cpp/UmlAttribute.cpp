
#include "UmlAttribute.h"

#include "CppSettings.h"
#include "JavaSettings.h"
#include "PhpSettings.h"
#include "PythonSettings.h"
QCString UmlAttribute::sKind() {
  return "attribute";
}

void UmlAttribute::memo_ref() {
  if (visibility() == PublicVisibility) {
   
    QCString s = parent()->stereotype();
    
    if ((s != "enum") && (s != "enum_pattern"))
      attrs.addElement(this);
  }
  UmlItem::memo_ref();
}

void UmlAttribute::html(QCString, unsigned int, unsigned int) {
  define();

  fw.write("<table><tr><td><div class=\"element\">Attribute <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>\n");

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
  
  annotation_constraint();
  write_properties();
 
  unload(FALSE, FALSE);
}

void UmlAttribute::ref_index()
{
  if (!attrs.isEmpty())
    fw.write("<a href=\"public_properties.html\" target = \"projectFrame\"><b> -Public properties- </b></a>");
}

void UmlAttribute::generate_index()
{
  unsigned n = attrs.size();
  
  if (n != 0) {
    sort(attrs);
    
    start_file("public_properties", "Public Properties Index", TRUE);
    
    fw.write("<table>\n");
    fw.write("<tr bgcolor=\"#f0f0f0\"><td align=\"center\"><b>Property</b></td><td align=\"center\"><b>Class</b></td><td align=\"center\"><b>Description</b></td></tr>\n");
      
    for (unsigned i = 0; i != n; i += 1) {
      UmlItem * prop = attrs.elementAt(i);
      
      fw.write("<tr bgcolor=\"#f0f0f0\"><td>");
      prop->write();
      fw.write("</td><td>");
      prop->parent()->write();
      fw.write("</td><td>");
      writeq(prop->description());
      fw.write("</td></tr>\n");
    }
    fw.write("</table>\n");
    
    end_file();
  }
}

void UmlAttribute::gen_cpp_decl(QCString s, bool descr) {
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
  
  for (;;) {
    switch (*p) {
    case 0:
      return;
    case '$':
      if (!strncmp(p, "${comment}", 10))
	p += 10;
      else if (!strncmp(p, "${description}", 14))
	p += 14;
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	writeq(name());
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
      else if (!strncmp(p, "${value}", 8) || !strncmp(p, "${h_value}", 10))
	return;
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
	write(type(), cppLanguage);
      }
      else
	fw.write(*p++);
      break;
    case '\r':
      p += 1;
      break;
    case '\n':
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
      break;
    case ';':
      if (descr)
	fw.write(*p++);
      else
	return;
      break;
    case '@':
      manage_alias(p);
      break;
    default:
      writeq(*p++);
    }
  }
}

void UmlAttribute::gen_java_decl(QCString s, bool descr) {
  const char * p = bypass_comment(s);

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(name());
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
    else if (!strncmp(p, "${class}", 8)) {
      p += 8;
      writeq(parent()->name());
    }
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
      write(type(), javaLanguage);
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
    else if ((*p == '{') || (*p == ';')) {
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

void UmlAttribute::gen_php_decl(QCString s, bool descr) {
  QCString st = PhpSettings::classStereotype(stereotype());
  const char * p = bypass_comment(s);

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      if ((st != "enum") && !isReadOnly())
	fw.write('$');
      writeq(name());
    }
    else if (!strncmp(p, "${var}", 6)) {
      p += 6;
      if ((st != "enum") &&
	  !isReadOnly() &&
	  !isClassMember() &&
	  (visibility() == PackageVisibility))
	fw.write("var ");
    }
    else if (!strncmp(p, "${value}", 8)) {
      p += 8;
    }
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
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      write(type(), phpLanguage);
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
    else if ((*p == '{') || (*p == ';')){
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

void UmlAttribute::gen_python_decl(QCString s) {
  QCString st = PythonSettings::classStereotype(stereotype());
  const char * p = bypass_comment(s);

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(name());
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
      write(type(), pythonLanguage);
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

void UmlAttribute::gen_uml_decl() {
  if (isClassMember())
    fw.write("static, ");
  write(visibility());
  writeq(name());
  fw.write(" : ");
  write(type());
  
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

Vector UmlAttribute::attrs;

