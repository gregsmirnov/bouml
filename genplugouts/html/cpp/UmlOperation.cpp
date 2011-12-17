
#include "UmlOperation.h"

#include "CppSettings.h"
#include "JavaSettings.h"
#include "PhpSettings.h"
#include "PythonSettings.h"
#include "UmlRelation.h"

QCString UmlOperation::sKind() {
  return "operation";
}

void UmlOperation::memo_ref() {
  if (visibility() == PublicVisibility)
    opers.addElement(this);
  UmlItem::memo_ref();
}

void UmlOperation::html(QCString, unsigned int, unsigned int) {
  define();

  fw.write("<table><tr><td><div class=\"element\">Operation <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>");

  QCString s = description();
  
  if (! s.isEmpty()) {
    fw.write("<p>");
    if (! javaDecl().isEmpty())
      gen_java_decl(s, TRUE);
    else if (! phpDecl().isEmpty())
      gen_php_decl(s, TRUE);
    else if (! pythonDecl().isEmpty())
      gen_python_decl(s, TRUE);
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
    gen_python_decl(s, FALSE);
    fw.write("</li>");
  }

  fw.write("</ul>");
  
  annotation_constraint();
  write_properties();
 
  unload(FALSE, FALSE);
}

void UmlOperation::ref_index()
{
  if (!opers.isEmpty())
    fw.write("<a href=\"public_operations.html\" target = \"projectFrame\"><b> -Public Operations- </b></a>");
}

void UmlOperation::generate_index()
{
  unsigned n = opers.size();
  
  if (n != 0) {
    sort(opers);
    
    start_file("public_operations", "Public Operations Index", TRUE);
    
    fw.write("<table>\n");
    fw.write("<tr bgcolor=\"#f0f0f0\"><td align=\"center\"><b>Operation</b></td><td align=\"center\"><b>Class</b></td><td align=\"center\"><b>Description</b></td></tr>\n");
      
    for (unsigned i = 0; i != n; i += 1) {
      UmlItem * op = opers.elementAt(i);
      
      fw.write("<tr bgcolor=\"#f0f0f0\"><td>");
      op->write();
      fw.write("</td><td>");
      op->parent()->write();
      fw.write("</td><td>");
      writeq(op->description());
      fw.write("</td></tr>\n");
    }
    fw.write("</table>\n");
    
    end_file();
  }

}

void UmlOperation::gen_uml_decl() {
  if (isAbstract())
    fw.write("abstract, ");
  if (isClassMember())
    fw.write("static, ");
  write(visibility());
  writeq(name());
  
  const QValueList<UmlParameter> & pa = params();
  unsigned npa = pa.count();
  unsigned rank;
  const char * sep = "(";
  
  for (rank = 0; rank != npa; rank += 1) {
    const UmlParameter & p = pa[rank];
    
    fw.write(sep);
    sep = ", ";
    
    switch (p.dir) {
    case InputOutputDirection:
      fw.write("inout ");
      break;
    case InputDirection:
      fw.write("in ");
      break;
    default:
      // OutputDirection
      fw.write("out ");
    }
    writeq(p.name);
    fw.write(" : ");
    write(p.type);
    
    QCString s = p.default_value;
    
    if (!s.isEmpty()) {
      if (s[0] != '=')
	fw.write(" = ");
      writeq(s);
    }
  }

  fw.write((rank == 0) ? "() : " : ") : ");
  write(returnType());
  
  sep = ",  exceptions : ";
  
  const QValueList<UmlTypeSpec> e = exceptions();
  unsigned n = e.count();
  
  for (unsigned index2 = 0; index2 != n; index2 += 1) {
    fw.write(sep);
    sep = ", ";
    write(e[index2]);
  }
}

void UmlOperation::gen_cpp_decl(QCString s, bool descr) {
  const char * p = bypass_comment(s);
  
  if (! descr) {
    write((cppVisibility() == DefaultVisibility)
	  ? visibility() : cppVisibility(),
	  cppLanguage);
    fw.write(": ");
    p = bypass_comment(s);
  }
  else
    p = s;

  const QValueList<UmlParameter> & pa = params();
  unsigned npa = pa.count();
  unsigned rank;

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${friend}", 9)) {
      p += 9;
      if (isCppFriend())
	fw.write("friend ");
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (isClassMember())
	fw.write("static ");
    }
    else if (!strncmp(p, "${inline}", 9)) {
      p += 9;
      if (isCppInline())
	fw.write("inline ");
    }
    else if (!strncmp(p, "${virtual}", 10)) {
      p += 10;
      if (isCppVirtual())
	fw.write("virtual ");
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      write(returnType(), cppLanguage);
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(compute_name(cppNameSpec()));
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      fw.write('(');
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      fw.write(')');
    }
    else if (!strncmp(p, "${const}", 8)) {
      p += 8;
      if (isCppConst())
	fw.write(" const");
    }
    else if (!strncmp(p, "${volatile}", 11)) {
      p += 11;
      if (isVolatile())
	fw.write(" volatile");
    }
    else if (!strncmp(p, "${throw}", 8)) {
      p += 8;
      
      const char * sep = " throw (";
      QValueList<UmlTypeSpec> e = exceptions();
      unsigned n = e.count();
      unsigned index2;
      
      for (index2 = 0; index2 != n; index2 += 1) {
	fw.write(sep);
	sep = ", ";
	write(e[index2], cppLanguage);
      }
      if (index2 != 0)
	fw.write(')');
      else if (CppSettings::operationForceThrow())
	fw.write(" throw ()");
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank < npa) 
	write(pa[rank].type, cppLanguage);
      else
	fw.write("???");
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;
      
      if (rank < pa.count()) 
	writeq(pa[rank].name);
      else
	fw.write("???");
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;
      
      if (rank >= pa.count())
	fw.write("???");
      else if (! pa[rank].default_value.isEmpty()) {
	fw.write(" = ");
	writeq(pa[rank].default_value);
      }
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      if (isAbstract())
	fw.write("= 0 ");
      break;
    }
    else if (!strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set relation with multiplicity > 1
      UmlClassMember * m = getOf();
      
      if ((m != 0) || ((m = setOf()) != 0))
	writeq(CppSettings::relationAttributeStereotype(m->stereotype()));
    }
    else if (!strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set relation with multiplicity > 1
      UmlClassMember * m = getOf();
      
      if (((m != 0) || ((m = setOf()) != 0)) &&
	  (m->kind() == aRelation))
	write(((UmlRelation *) m)->association(), cppLanguage);
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

void UmlOperation::gen_java_decl(QCString s, bool descr) {
  const char * p = bypass_comment(s);
  const QValueList<UmlParameter> & pa = params();
  unsigned npa = pa.count();
  unsigned rank;

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (isJavaFinal())
	fw.write("final ");
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      UmlItem::write(visibility(), javaLanguage);
      fw.write(' ');
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (isClassMember())
	fw.write("static ");
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (isAbstract())
	fw.write("abstract ");
    }
    else if (!strncmp(p, "${synchronized}", 15)) {
      p += 15;
      if (isJavaSynchronized())
	fw.write("synchronized ");
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      write(returnType(), javaLanguage);
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(compute_name(javaNameSpec()));
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      fw.write('(');
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      fw.write(')');
    }
    else if (!strncmp(p, "${throws}", 9)) {
      p += 9;
      
      const char * sep = " throws ";
      const QValueList<UmlTypeSpec> e = exceptions();
      unsigned n = e.count();
      
      for (unsigned index2 = 0; index2 != n; index2 += 1) {
	fw.write(sep);
	sep = ", ";
	write(e[index2], javaLanguage);
      }
    }
    else if (!strncmp(p, "${staticnl}", 11))
      break;
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank < npa)
	write(pa[rank].type, javaLanguage);
      else
	fw.write("???");
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank < npa)
	writeq(pa[rank].name);
      else
	fw.write("???");
    }
    else if (!strncmp(p, "${stereotype}", 13)) {
      p += 13;
      // get/set relation with multiplicity > 1
      UmlClassMember * m = getOf();
      
      if ((m != 0) || ((m = setOf()) != 0))
	writeq(JavaSettings::relationAttributeStereotype(m->stereotype()));
    }
    else if (!strncmp(p, "${association}", 14)) {
      p += 14;
      // get/set relation with multiplicity > 1
      UmlClassMember * m = getOf();
      
      if (((m != 0) || ((m = setOf()) != 0)) &&
	  (m->kind() == aRelation))
	write(((UmlRelation *) m)->association(), javaLanguage);
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

void UmlOperation::gen_php_decl(QCString s, bool descr) {
  QCString cl_stereotype = 
     PhpSettings::classStereotype(parent()->stereotype());
  const char * p = bypass_comment(s);
  const QValueList<UmlParameter> & pa = params();
  unsigned npa = pa.count();
  unsigned rank;

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (isPhpFinal())
	fw.write("final ");
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      UmlItem::write(visibility(), phpLanguage);
      fw.write(' ');
    }
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (isClassMember())
	fw.write("static ");
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (isAbstract() && (cl_stereotype != "interface"))
	fw.write("abstract ");
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(compute_name(phpNameSpec()));
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      fw.write('(');
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      fw.write(')');
    }
    else if (!strncmp(p, "${staticnl}", 11))
      break;
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank < npa)
	write(pa[rank].type, phpLanguage);
      else
	fw.write("???");
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank < npa) {
	fw.write('$');
	writeq(pa[rank].name);
      }
      else
	fw.write("???");
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank >= npa)
	fw.write("???");
      else if (! pa[rank].default_value.isEmpty()) {
	fw.write(" = ");
	writeq(pa[rank].default_value);
      }
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

void UmlOperation::gen_python_decl(QCString s, bool descr) {
  QCString cl_stereotype = 
     PythonSettings::classStereotype(parent()->stereotype());
  const char * p = bypass_comment(s);
  const QValueList<UmlParameter> & pa = params();
  unsigned npa = pa.count();
  bool in_params = FALSE;
  unsigned rank;

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${docstring}", 12))
      p += 12;
    else if (!strncmp(p, "${static}", 9)) {
      p += 9;
      if (isClassMember())
	fw.write("@staticmethod<br />");
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (isAbstract())
	fw.write("@abstractmethod<br />");
    }
    else if (!strncmp(p, "${@}", 4)) {
      p += 4;
      writeq(pythonDecorators());
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(compute_name(pythonNameSpec()));
    }
    else if (!strncmp(p, "${class}", 8)) {
      p += 8;
      parent()->write();
    }
    else if (!strncmp(p, "${(}", 4)) {
      p += 4;
      fw.write('(');
      in_params = TRUE;
    }
    else if (!strncmp(p, "${)}", 4)) {
      p += 4;
      fw.write(')');
      in_params = FALSE;
    }
    else if (sscanf(p, "${t%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank < npa) {
	const UmlTypeSpec & t = pa[rank].type;
	
	if (! t.toString().isEmpty()) {
	  if (in_params)
	    fw.write(": ");
	  write(t, pythonLanguage);
	}
      }
      else
	fw.write("???");
    }
    else if (sscanf(p, "${p%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank < npa) {
	fw.write('$');
	writeq(pa[rank].name);
      }
      else
	fw.write("???");
    }
    else if (sscanf(p, "${v%u}", &rank) == 1) {
      p = strchr(p, '}') + 1;

      if (rank >= npa)
	fw.write("???");
      else if (! pa[rank].default_value.isEmpty()) {
	fw.write(" = ");
	writeq(pa[rank].default_value);
      }
    }
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      
      const UmlTypeSpec & t = returnType();
	
      if (! t.toString().isEmpty()) {
	fw.write(" -> ");
	write(t, pythonLanguage);
      }
    }
    else if (*p == ':') {
      if (descr)
	fw.write(*p++);
      else
	break;
    }
    else if (*p == '\r')
      p += 1;
    else if (*p == '@')
      manage_alias(p);
    else
      writeq(*p++);
  }
}

QCString UmlOperation::compute_name(QCString s) {
  if (!s.isEmpty()) {
    UmlClassMember * m = getOf();
    
    if ((m != 0) || ((m = setOf()) != 0)) {
      QCString n = (m->kind() == aRelation)
	? ((UmlRelation *) m)->roleName()
	: m->name();
      int index;
      
      if ((index = s.find("${name}")) != -1)
	return s.left(index) + n + s.mid(index + 7);
      else if ((index = s.find("${Name}")) != -1)
	return s.left(index) + n.left(1).upper() + n.mid(1) + s.mid(index + 7);
      else if ((index = s.find("${NAME}")) != -1)
	return s.left(index) + n.upper() + s.mid(index + 7);
      else
	return s;
    }
  }

  return name();
}

Vector UmlOperation::opers;

