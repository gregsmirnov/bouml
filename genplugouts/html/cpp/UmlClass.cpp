
#include "UmlClass.h"
#include "UmlActualParameter.h"
#include "UmlFormalParameter.h"

#include "JavaSettings.h"
#include "PhpSettings.h"
#include "PythonSettings.h"
#include "UmlArtifact.h"
#include "UmlComponent.h"
#include "UmlClassDiagram.h"
#include "UmlOperation.h"
#include "UmlRelation.h"
#include "UmlCom.h"

QCString UmlClass::sKind() {
  return (stereotype() == "stereotype") 
    ? "stereotype" : "class";
}

void UmlClass::memo_ref() {
  classes.addElement(this);
  UmlItem::memo_ref();
  
  const QVector<UmlItem> ch = children();
  
  if (inherited_opers == 0)
    add_inherited_opers(0);
	
  for (unsigned i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == aRelation) {
      UmlRelation * rel = (UmlRelation *) ch[i];
      aRelationKind k = rel->relationKind();
      
      if ((k == aGeneralisation) || (k == aRealization))
	rel->roleType()->subClasses.addElement(this);
    }
  }

  unload(TRUE, FALSE);
}

void UmlClass::html(QCString pfix, unsigned int rank, unsigned int level) {
  QCString s = stereotype();
  
  if (flat) {
    define();
    if (s == "stereotype")
      chapter("Stereotype", pfix, rank, "stereotype", level);
    else if (s == "metaclass")
      chapter("Metaclass", pfix, rank, "metaclass", level);
    else
      chapter("Class", pfix, rank, "class", level);
    gen_html(pfix, rank, level);
    unload(FALSE, FALSE);
  }
  else {
    if (s == "stereotype")
      fw.write("<table><tr><td><div class=\"element\">Stereotype <b>");
    else if (s == "metaclass")
      fw.write("<table><tr><td><div class=\"element\">Metaclass <b>");
    else
      fw.write("<table><tr><td><div class=\"element\">Class <b>");
    write();
    fw.write("</b></div></td></tr></table>\n");
  }
}

void UmlClass::html() {
  QCString s;
  
  UmlCom::message(name());
  
  if (stereotype() == "stereotype")
    start_file("stereotype" + s.setNum((unsigned) getIdentifier()), "Stereotype " + name(), TRUE);
  else if (stereotype() == "metaclass")
    start_file("metaclass" + s.setNum((unsigned) getIdentifier()), "Metaclass " + name(), TRUE);
  else
    start_file("class" + s.setNum((unsigned) getIdentifier()), "Class " + name(), TRUE);
  define();
  gen_html("", 0, 0);
  end_file();
  
  unload(FALSE, FALSE);
}

void UmlClass::gen_html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlCom::message(name());
  
  QCString s;
 
  s = description();
  
  if (isActive())
    fw.write("<p>Active class</p>\n");
    
  if (!s.isEmpty()) {
    fw.write("<p>");
    if (! javaDecl().isEmpty())
      gen_java_decl(s, TRUE);
    else if (! phpDecl().isEmpty())
      gen_php_decl(s, TRUE);
    else if (! pythonDecl().isEmpty())
      gen_python_decl(s, TRUE);
    else
      gen_cpp_decl(s, TRUE);
    fw.write("<br /></p>\n");
  }

  if (!cppDecl().isEmpty() ||
      !javaDecl().isEmpty() ||
      !phpDecl().isEmpty() ||
      !pythonDecl().isEmpty()) {
    fw.write("<p>Declaration :</p><ul>\n");
    
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
  }
  
  if (subClasses.size() != 0) {
    sort(subClasses);
    fw.write("<p>Directly inherited by : ");
    
    for (unsigned i = 0; i != subClasses.size(); i += 1) {
      subClasses.elementAt(i)->write();
      fw.write(' ');
    }
    fw.write("</p>\n");    
  }
  
  write_dependencies();

  annotation_constraint();
  
  bool p = FALSE;
  UmlItem * x;
  
  if ((x = associatedArtifact()) != 0) {
    p = TRUE;
    fw.write("<p>Artifact : ");
    x->write();
  }

  const QVector<UmlComponent> comps = associatedComponents();
  
  if (comps.size() != 0) {
    if (p) 
      fw.write(", Component(s) :");
    else {
      p = TRUE;
      fw.write("<p>Component(s) :");
    }
    
    for (unsigned i = 0; i != comps.size(); i += 1) {
      fw.write(' ');
      comps[i]->write();
    }
  }

  if ((x = associatedDiagram()) != 0) {
    if (p) 
      fw.write(", Diagram : ");
    else {
      p = TRUE;
      fw.write("<p>Diagram : ");
    }
    x->write();
  }

  if (p)
    fw.write("</p>\n");
  
  if (parent()->kind() == aClass) {
    fw.write("<p>nested in ");
    parent()->write();
    fw.write("</p>\n");
  }

  write_properties();

  //

  const QVector<UmlItem> ch = children();
  
  if (ch.size() != 0) {
    if (stereotype() == "enum_pattern") {
      p = FALSE;
            
      for (unsigned i = 0; i != ch.size(); i += 1) {
	if (ch[i]->kind() == anAttribute) {
	  if (!p) {
	    p = TRUE;
	    fw.write("<div class=\"sub\">\n<p>Items :</p><ul>\n");
	  }
	  fw.write("<li>");
	  writeq(ch[i]->name());
	  fw.write("</li>\n");
	}
      }
      if (p)
	fw.write("</ul>\n</div>\n");
    }
    else {
      fw.write("<div class=\"sub\">\n");
      
      if (stereotype() == "enum") {
	unsigned i;
	
	p = FALSE;
	
	for (i = 0; i != ch.size(); i += 1) {
	  if ((ch[i]->kind() == anAttribute) &&
	      (ch[i]->stereotype() != "attribute")) {
	    if (!p) {
	      p = TRUE;
	      fw.write("<p>Items :</p><ul>\n");
	    }
	    fw.write("<li>");
	    writeq(ch[i]->name());
	    fw.write("</li>\n");
	  }
	}
	if (p)
	  fw.write("</ul>\n");
	
	s = "";
	for (i = 0; i != ch.size(); i += 1)
	  if ((ch[i]->kind() != anAttribute) ||
	      (ch[i]->stereotype() == "attribute"))
	    ch[i]->html(s, 0, 0);
      }
      else if (flat)
	write_children(pfix, rank, level);
      else {
	// non flat
	s = "";
	for (unsigned i = 0; i != ch.size(); i += 1)
	  ch[i]->html(s, 0, 0);
      }
      
      fw.write("</div>\n");
    }
  }

  sort(*inherited_opers);
  bool already = FALSE;
  
  for (unsigned i = 0; i != inherited_opers->size(); i += 1) {
    if (already)
      fw.write(", ");
    else {
      already = TRUE;
      fw.write("<p>All public operations : ");
    }
    inherited_opers->elementAt(i)->write();
    fw.write(' ');
  }
  if (already)
    fw.write("</p>\n");
}

void UmlClass::write() {
  if (!known)
    writeq(name());
  else {
    QCString s = stereotype();
    
    if ((s != "stereotype") && (s != "metaclass"))
      s = "class";
    
    if (flat)
      fw.write("<a href=\"index");
    else {
      fw.write("<a href=\"");
      fw.write(s);
      fw.write((unsigned) getIdentifier());
    }
    fw.write(".html#ref");
    fw.write(s);
    fw.write((unsigned) getIdentifier());
    fw.write("\"><b>");
    writeq(name());
    fw.write("</b></a>");
  }
}

void UmlClass::write(QCString target) {
  if (known) {
    QCString s = stereotype();
    
    if ((s != "stereotype") && (s != "metaclass"))
      s = "class";
    
    if (flat)
      fw.write("<a href=\"index");
    else {
      fw.write("<a href=\"");
      fw.write(s);
      fw.write((unsigned) getIdentifier());
    }
    fw.write(".html#ref");
    fw.write(s);
    fw.write((unsigned) getIdentifier());
    fw.write("\" target = \"");
    fw.write(target);
    fw.write("\"><b>");
    writeq(name());
    fw.write("</b></a>");
  }
  else
    writeq(name());
}

void UmlClass::ref_index()
{
  if (!classes.isEmpty())
    fw.write("<a href=\"classes.html\" target = \"projectFrame\"><b> -Classes- </b></a>");
}

void UmlClass::generate_index()
{
  UmlItem::generate_index(classes, "Classes", "classes");

  int n = classes.size();
  
  start_file("classes_list", "Classes", FALSE);
  
  fw.write("<table border=\"0\" width=\"100%\">\n<tr>\n<td nowrap=\"nowrap\">");
  
  for (int i = 0; i != n; i += 1) {
    classes.elementAt(i)->write("projectFrame");
    fw.write("<br />\n");
  }
  
  fw.write("</td>\n</tr>\n</table>\n");
  end_file();

}

void UmlClass::add_inherited_opers(Vector * ops) {
  if (inherited_opers == 0) {
    const QVector<UmlItem> ch = children();
    
    inherited_opers = new Vector;
	
    for (unsigned i = 0; i != ch.size(); i += 1) {
      switch (ch[i]->kind()) {
      case aRelation:
	{
	  UmlRelation * rel = (UmlRelation *) ch[i];
	  aRelationKind k = rel->relationKind();
	  
	  if ((k == aGeneralisation) || (k == aRealization))
	    rel->roleType()->add_inherited_opers(inherited_opers);
	}
	break;
      case anOperation:
	{
	  UmlOperation * op = (UmlOperation *) ch[i];
	  
	  if ((op->visibility() == PublicVisibility) &&
	      (op->name()[0] != '~') &&
	      (op->name() != name()))
	    inherited_opers->addElement(op);
	}
      default:
	break;
      }
    }
  }

  if (ops != 0)
    for (unsigned i = 0; i != inherited_opers->size(); i += 1)
      if (! ops->contains(inherited_opers->elementAt(i)))
	ops->addElement(inherited_opers->elementAt(i));

  unload(TRUE, FALSE);
}

Vector UmlClass::classes;

void UmlClass::gen_cpp_decl(QCString s, bool descr) {
  const char * p = (descr)
    ? (const char *) s
    : (const char *) bypass_comment(s);

  while (*p) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${type}", 7)) {
      p += 7;
      bool find = FALSE;

      if (baseType().type != 0) {
	UmlClass * mother = baseType().type;
	const QVector<UmlItem> ch = children();
	
	for (unsigned i = 0; i != ch.size(); i += 1) {
	  if (ch[i]->kind() == aRelation) {
	    UmlRelation * rel = (UmlRelation *) ch[i];
	    aRelationKind k = rel->relationKind();
	    
	    if (((k == aGeneralisation) ||
		 (k == aRealization)) &&
		(rel->roleType() == mother)) {
	      rel->roleType()->write();
	      generate(actuals(), mother, TRUE);
	      find = TRUE;
	      break;
	    }
	  }
	}
      }
      if (! find)
	UmlItem::write(baseType(), cppLanguage);
    }
    else if (!strncmp(p, "${template}", 11)) {
      p += 11;
      generate(formals());
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(name());
    }
    else if (!strncmp(p, "${inherit}", 10)) {
      p += 10;

      const QVector<UmlItem> ch = children();
      const char * sep = " : ";

      for (unsigned i = 0; i != ch.size(); i += 1) {
	if (ch[i]->kind() == aRelation) {
	  UmlRelation * rel = (UmlRelation *) ch[i];
	  aRelationKind k = rel->relationKind();
	  
	  if (((k == aGeneralisation) ||
	       (k == aRealization)) &&
	      !rel->cppDecl().isEmpty()) {
	    fw.write(sep);
	    // UmlItem::write else G++ call UmlClass::write(QCString) !
	    UmlItem::write((rel->cppVisibility() == DefaultVisibility)
		           ? rel->visibility() : rel->cppVisibility(),
			   cppLanguage);
	    fw.write((rel->cppVirtualInheritance()) ? " virtual " : " ");
	    rel->roleType()->write();
	    generate(actuals(), rel->roleType(), TRUE);
	    sep = ", ";
	  }
	}
      }
    }
    else if (*p == '{') {
      if (descr)
	fw.write(*p++);
      else
	break;
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
    else if (*p == '@')
      manage_alias(p);
    else
      writeq(*p++);
  }

}

void UmlClass::gen_java_decl(QCString s, bool descr) {
  const char * p = bypass_comment(s);
  UmlRelation * extend = 0;

  while (*p != 0) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${public}", 9)) {
      p += 9;
      if (isJavaPublic())
	fw.write("public ");
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      UmlItem::write(visibility(), javaLanguage);
      fw.write(' ');
    }
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (isJavaFinal())
	fw.write("final ");
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (isAbstract())
	fw.write("abstract ");
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(name());
      generics();
    }
    else if (!strncmp(p, "${extends}", 10)) {
      p += 10;

      const QVector<UmlItem> ch = children();

      for (unsigned i = 0; i != ch.size(); i += 1) {
	if (ch[i]->kind() == aRelation) {
	  UmlRelation * rel = (UmlRelation *) ch[i];
	  aRelationKind k = rel->relationKind();
	  
	  if (((k == aGeneralisation) ||
	       (k == aRealization)) &&
	      (!rel->javaDecl().isEmpty()) &&
	      ((JavaSettings::classStereotype(stereotype()) == "interface") ||
	       (JavaSettings::classStereotype(rel->roleType()->stereotype()) != "interface"))) {
	    extend = rel;
	    fw.write(" extends ");
	    rel->roleType()->write();
	    generate(actuals(), rel->roleType(), FALSE);
	    break;
	  }
	}
      }
    }
    else if (!strncmp(p, "${implements}", 13)) {
      p += 13;

      const QVector<UmlItem> ch = children();
      const char * sep = " implements ";

      for (unsigned i = 0; i != ch.size(); i += 1) {
	if (ch[i]->kind() == aRelation) {
	  UmlRelation * rel = (UmlRelation *) ch[i];
	  aRelationKind k = rel->relationKind();
	  
	  if ((rel != extend) &&
	      ((k == aGeneralisation) ||
	       (k == aRealization)) &&
	      (!rel->javaDecl().isEmpty())) {
	    fw.write(sep);
	    sep = ", ";
	    rel->roleType()->write();
	    generate(actuals(), rel->roleType(), FALSE);
	  }
	}
      }
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

void UmlClass::gen_php_decl(QCString s, bool descr) {
  QCString st = PhpSettings::classStereotype(stereotype());
  
  if (st == "ignored")
    return;
    
  const char * p = bypass_comment(s);
  UmlRelation * extend = 0;

  while (*p != 0) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      UmlItem::write(visibility(), phpLanguage);
      fw.write(' ');
    }
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (isPhpFinal())
	fw.write("final ");
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (isAbstract())
	fw.write("abstract ");
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(name());
      generics();
    }
    else if (!strncmp(p, "${extends}", 10)) {
      p += 10;

      const QVector<UmlItem> ch = children();

      for (unsigned i = 0; i != ch.size(); i += 1) {
	if (ch[i]->kind() == aRelation) {
	  UmlRelation * rel = (UmlRelation *) ch[i];
	  aRelationKind k = rel->relationKind();
	  
	  if (((k == aGeneralisation) ||
	       (k == aRealization)) &&
	      (!rel->phpDecl().isEmpty()) &&
	      ((st == "interface") ||
	       (PhpSettings::classStereotype(rel->roleType()->stereotype()) != "interface"))) {
	    extend = rel;
	    fw.write(" extends ");
	    rel->roleType()->write();
	    break;
	  }
	}
      }
    }
    else if (!strncmp(p, "${implements}", 13)) {
      p += 13;

      const QVector<UmlItem> ch = children();
      const char * sep = " implements ";

      for (unsigned i = 0; i != ch.size(); i += 1) {
	if (ch[i]->kind() == aRelation) {
	  UmlRelation * rel = (UmlRelation *) ch[i];
	  aRelationKind k = rel->relationKind();
	  
	  if ((rel != extend) &&
	      ((k == aGeneralisation) ||
	       (k == aRealization)) &&
	      (!rel->phpDecl().isEmpty())) {
	    fw.write(sep);
	    sep = ", ";
	    rel->roleType()->write();
	  }
	}
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

void UmlClass::gen_python_decl(QCString s, bool descr) {
  QCString st = PythonSettings::classStereotype(stereotype());
  
  if (st == "ignored")
    return;
    
  const char * p = bypass_comment(s);

  while (*p != 0) {
    if (!strncmp(p, "${comment}", 10))
      p += 10;
    else if (!strncmp(p, "${description}", 14))
      p += 14;
    else if (!strncmp(p, "${docstring}", 12))
      p += 12;
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      writeq(name());
    }
    else if (!strncmp(p, "${inherit}", 10)) {
      p += 10;

      const QVector<UmlItem> ch = children();
      bool inh = FALSE;

      for (unsigned i = 0; i != ch.size(); i += 1) {
	if (ch[i]->kind() == aRelation) {
	  UmlRelation * rel = (UmlRelation *) ch[i];
	  aRelationKind k = rel->relationKind();
	  
	  if (((k == aGeneralisation) || (k == aRealization)) &&
	      !rel->pythonDecl().isEmpty()) {
	    if (inh)
	      fw.write(", ");
	    else  {
	      inh = TRUE;
	      fw.write('(');
	    }
	    rel->roleType()->write();
	  }
	}
      }
      
      if (inh)
	fw.write(')');
      else if (isPython_2_2())
	fw.write("(object)");
      break;
    }
    else if (!descr && ((*p == '\r') || (*p == '\n') || (*p == ':')))
      break;
    else if (*p == '@')
      manage_alias(p);
    else
      writeq(*p++);
  }
}

void UmlClass::generate(const QValueList<UmlActualParameter> a, UmlClass * mother, bool cpp) {
  unsigned i;
  unsigned n = a.count();
  
  // search first actual of mother
  for (i = 0; ; i += 1) {
    if (i == n)
      return;
    if (a[i].superClass() == mother)
      break;
  }
    
  const char * sep = "<";
  aLanguage lang = (cpp) ? cppLanguage : javaLanguage;
  
  do {
    writeq(sep);
    UmlItem::write(a[i].value(), lang);
    sep = ", ";
  }
  while ((++i != n) && (a[i].superClass() == mother));

  writeq('>');
}

void UmlClass::generate(const QValueList<UmlFormalParameter> f) {
  unsigned n = f.count();
  
  if (n != 0) {
    const char * sep = "template<";
    unsigned i;
    
    for (i = 0; i != n; i += 1) {
      writeq(sep);
      writeq(f[i].type());
      fw.write(' ');
      writeq(f[i].name());
      sep = ", ";
    }
    
    writeq("> ");
  }
  else if (name().find("<") != -1)
    writeq("template<> ");
}

void UmlClass::generics() {
  QValueList<UmlFormalParameter> f = formals();
  unsigned n = f.count();
  
  if (n != 0) {
    const char * sep = "<";
    unsigned i;
    
    for (i = 0; i != n; i += 1) {
      writeq(sep);
      sep = ", ";
      writeq(f[i].name());
      
      const UmlTypeSpec & t = f[i].extend();
      
      if ((t.type != 0) || ! t.explicit_type.isEmpty()) {
	fw.write(" extends ");
	UmlItem::write(t, javaLanguage);
      }
    }
    
    writeq('>');
  }
}

void UmlClass::generate()
{
  if (! flat) {
    int n = classes.size();
    
    for (int i = 0; i != n; i += 1)
      ((UmlClass *) classes.elementAt(i))->html();
  }
}

bool UmlClass::chapterp() {
  return flat;
}

