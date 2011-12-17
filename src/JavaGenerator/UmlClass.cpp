// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#include <stdio.h>// debug

#include <qtextstream.h> 

#include "UmlClass.h"
#include "UmlPackage.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlArtifact.h"
#include "UmlSettings.h"
#include "JavaSettings.h"
#include "UmlCom.h"
#include "util.h"

static QCString dot(".");

QCString UmlClass::java_stereotype()
{
  QCString s = JavaSettings::classStereotype(stereotype());
  
  return ((s == "ignored") || (s == "union") || (s == "enum") ||
	  (s == "interface") || (s == "@interface") ||
	  (s == "enum_pattern"))
    ? s : QCString("class");
}

void UmlClass::generate() {
  if (! managed) {
    managed = TRUE;
    
    if (!isJavaExternal() && !javaDecl().isEmpty()) {
      if (associatedArtifact() != 0)
	associatedArtifact()->generate();
      else if ((children().size() != 0) && verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica><i> ") + name() +
		      " : </i> does not have associated <i>artifact</i></font><br>");
    }
  }
}

void UmlClass::generate(QTextOStream & f, QCString indent) {
  QVector<UmlItem> ch = children();
  const QValueList<UmlActualParameter> actuals = this->actuals();
  const unsigned sup = ch.size();
  const QCString & stereotype = java_stereotype();
  bool an_enum_pattern = (stereotype == "enum_pattern");
  bool an_enum = (stereotype == "enum");
  unsigned index;
  const char * p = javaDecl();
  const char * pp = 0;
  const char * sep;
  
  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  f << indent;
    
  for (;;) {
    if (*p == 0) {
      if (pp == 0)
	break;
      
      // comment management done
      p = pp;
      pp = 0;
      if (*p == 0)
	break;
      f << indent;
    }

    if (*p == '\n') {
      f << *p++;
      if (*p &&
	  strncmp(p, "${members}", 10) &&
	  strncmp(p, "${items}", 8) &&
	  strncmp(p, "${cases}", 8))
	f << indent;
    }
    else if (*p == '@')
      manage_alias(p, f);
    else if (*p != '$')
      f << *p++;
    else if (!strncmp(p, "${comment}", 10))
      manage_comment(p, pp, JavaSettings::isGenerateJavadocStyleComment());
    else if (!strncmp(p, "${description}", 14))
      manage_description(p, pp);
    else if (!strncmp(p, "${public}", 9)) {
      p += 9;
      if (visibility() == PublicVisibility)
	f << "public ";
    }
    else if (!strncmp(p, "${visibility}", 13)) {
      p += 13;
      generate_visibility(f, "");
    }
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (isJavaFinal())
	f << "final ";
    }
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (isAbstract())
	f << "abstract ";
    }
    else if (! strncmp(p, "${name}", 7)) {
      p += 7;
      f << name();
      generate_formals(f);
    }
    else if (!strncmp(p, "${@}", 4)) {
      p += 4;
      if (pp != 0)
	f << "${@}";
      else if (! javaAnnotations().isEmpty()) {
	pp = p;
	p = javaAnnotations();
      }
    }
    else if (an_enum_pattern) {
      if (!strncmp(p, "${members}", 10)) {
	p += 10;
	
	int current_value = 0;
	QCString name = this->name();
	
	for (index = 0; index != sup; index += 1)
	  if ((ch[index]->kind() != aNcRelation) &&
	      !((UmlClassItem *) ch[index])->javaDecl().isEmpty())
	    ((UmlClassItem *) ch[index])->
	      generate_enum_pattern_item(f, current_value, name, indent);
      
	if (*p == '}')
	  f << indent;
      }
      else if (!strncmp(p, "${cases}", 8)) {
	p += 8;
	
	for (index = 0; index != sup; index += 1)
	  if ((ch[index]->kind() != aNcRelation) &&
	      !((UmlClassItem *) ch[index])->javaDecl().isEmpty())
	    ((UmlClassItem *) ch[index])->generate_enum_pattern_case(f, indent);
      }
      else
	// strange
	f << *p++;
    }
    else if (! strncmp(p, "${extends}", 10)) {
      p += 10;

      // extends
	
      sep = " extends ";
      
      for (index = 0; index != sup; index += 1) {
	UmlItem * x = ch[index];
	
	if (x->kind() == aRelation)
	  ((UmlRelation *) x)->generate_extends(sep, f, actuals, stereotype);
      }
    }
    else if (! strncmp(p, "${implements}", 13)) {
      p += 13;

      if (stereotype != "interface") {
	
	// implements
	
	sep = " implements ";
	
	for (index = 0; index != sup; index += 1) {
	  UmlItem * x = ch[index];
	  
	  if (x->kind() == aRelation)
	    ((UmlRelation *) x)->generate_implements(sep, f, actuals, stereotype);
	}
      }
    }
    else if (! strncmp(p, "${members}", 10)) {
      p += 10;
  
      // members
      
      if (an_enum) {
	for (index = 0; index != sup; index += 1) {
	  if (ch[index]->kind() != aNcRelation) {
	    UmlClassItem * it = (UmlClassItem *)ch[index];
	    
	    if (! it->javaDecl().isEmpty())
	      it->generate_enum_member(f, indent);
	  }
	}
      }
      else {
	for (index = 0; index != sup; index += 1) {
	  UmlItem * it = ch[index];
	  
	  if (it->kind() == aClass) {
	    if (! ((UmlClass *) it)->javaDecl().isEmpty()) {
	      ((UmlClass *) it)->generate(f, indent + "  ");
	      f << '\n';
	    }
	  }
	  else if ((it->kind() != aNcRelation) &&
		   !((UmlClassItem *) it)->javaDecl().isEmpty())
	    ((UmlClassItem *) it)->generate(f, stereotype, indent);
	}
      }
      
      if (*p == '}')
	f << indent;
    }
    else if (an_enum && ! strncmp(p, "${items}", 8)) {
      p += 8;
  
      // enums items
      
      BooL first = TRUE;
      
      for (index = 0; index != sup; index += 1) {
	if (ch[index]->kind() != aNcRelation) {
	  UmlClassItem * it = (UmlClassItem *)ch[index];
	  
	  if (! it->javaDecl().isEmpty())
	    it->generate_enum_item(f, indent, first);
	}
      }
      
      if (*p == '}')
	f << indent;
    }
    else
      // strange
      f << *p++;
  }
}

void UmlClass::generate(QTextOStream & f, const QCString &,
			QCString indent) {
  generate(f, indent);
}

void UmlClass::write(QTextOStream & f, const UmlTypeSpec & t)
{
  if (t.type != 0)
    t.type->write(f);
  else
    f << JavaSettings::type(t.explicit_type);
}

void UmlClass::write(QTextOStream & f) {
  if (isJavaExternal()) {
    QCString s = javaDecl().stripWhiteSpace();
    int index;
      
    if ((index = s.find("${name}")) != -1)
      s.replace(index, 7, name());
    else if ((index = s.find("${Name}")) != -1)
      s.replace(index, 7, capitalize(name()));
    else if ((index = s.find("${NAME}")) != -1)
      s.replace(index, 7, name().upper());
    else if ((index = s.find("${nAME}")) != -1)
      s.replace(index, 7, name().lower());
    
    f << s;
  }
  else {
    UmlClass * toplevel = this;
    UmlItem * p;
    QCString s2;
    
    while ((p = toplevel->parent())->kind() == aClass) {
      toplevel = (UmlClass *) p;
      s2 = dot + p->name() + s2;
    }
    
    UmlArtifact * cp = toplevel->associatedArtifact();
    UmlPackage * pack = (UmlPackage *)
      ((cp != 0) ? (UmlItem *) cp : (UmlItem *) toplevel)->package();
    
    if (pack != UmlArtifact::generation_package()) {
      QCString s = pack->javaPackage();

      if (! s.isEmpty() && (s != "java.lang") && (s.left(10) != "java.lang.")) {
	s += s2;
	
	if (JavaSettings::isForcePackagePrefixGeneration() ||
	    !UmlArtifact::generated_one()->is_imported(s, name()))
	  f << s << '.';
      }
    }
    else if (! s2.isEmpty())
      f << s2.mid(1) << '.';
    
    f << name();
  }
}

void UmlClass::import(QTextOStream & f, const QCString & indent) {
  QCString s;
  
  if (!isJavaExternal()) {
    UmlArtifact * cp = associatedArtifact();
    UmlPackage * pack = (UmlPackage *)
      ((cp != 0) ? (UmlItem *) cp : (UmlItem *) this)->package();
    
    if ((s = pack->javaPackage()).isEmpty())
      return;
    
    QCString s2 = name();
    UmlItem * p = this;
    
    while ((p = p->parent())->kind() == aClass)
      s2 = p->name() + dot + s2;
    
    s += dot + s2;
  }
  else if ((s = package()->javaPackage()).isEmpty())
    return;
  else
    s += dot + name();
  
  if (! UmlArtifact::generated_one()->is_imported(s)) {
    f << indent << "import " << s << ";\n";
    UmlArtifact::generated_one()->imported(s);
  }
}

void UmlClass::generate_enum_pattern_item(QTextOStream &, int &,
					  const QCString &, QCString) {
  write_trace_header();
  UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>enum pattern</i> cannot have sub-class</b></font><br>");
  incr_warning();
}

void UmlClass::generate_enum_pattern_case(QTextOStream &, QCString) {
  // error already signaled
}

void UmlClass::generate_enum_member(QTextOStream & f, QCString indent) {
  generate(f, indent + "  ");
  f << '\n';
}

void UmlClass::generate_formals(QTextOStream & f) {
  QValueList<UmlFormalParameter> fs = formals();
  
  if (! fs.isEmpty()) {
    QValueList<UmlFormalParameter>::Iterator it;
    const char * sep = "<";
    
    for (it = fs.begin(); it != fs.end(); it++) {
      UmlFormalParameter & p = *it;
      
      f << sep;
      sep = ", ";
      f << p.name();
      
      const UmlTypeSpec & t = p.extend();
      
      if (t.type != 0) {
	f << " extends ";
	t.type->write(f);
      }
      else if (! t.explicit_type.isEmpty())
	f << " extends " << JavaSettings::type(t.explicit_type);
    }
    
    f << ">";
  }
}

void UmlClass::generate_import(QTextOStream & f, const QCString & indent) {
  QVector<UmlItem> ch = children();
  const unsigned sup = ch.size();
  unsigned index;
  
  for (index = 0; index != sup; index += 1)
    ch[index]->generate_import(f, indent);
}
