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
#include "PhpSettings.h"
#include "UmlCom.h"
#include "util.h"

QCString UmlClass::php_stereotype()
{
  QCString s = PhpSettings::classStereotype(stereotype());
  
  return ((s == "ignored") || (s == "enum") || (s == "interface"))
    ? s : QCString("class");
}

void UmlClass::generate() {
  if (! managed) {
    managed = TRUE;
    
    if (!isPhpExternal() && !phpDecl().isEmpty()) {
      if (associatedArtifact() != 0)
	associatedArtifact()->generate();
      else if ((children().size() != 0) && verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica><i> ") + name() +
		      " : </i> does not have associated <i>artifact</i></font><br>");
    }
  }
}

void UmlClass::generate(QTextOStream & f, QCString indent) {
  const QCString & stereotype = php_stereotype();
  
  if (stereotype == "ignored")
    return;
  
  QVector<UmlItem> ch = children();
  const unsigned sup = ch.size();
  bool an_enum = (stereotype == "enum");
  int enum_item_rank = 0;
  unsigned index;
  const char * p = phpDecl();
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
      if (*p && strncmp(p, "${members}", 10) && strncmp(p, "${items}", 8))
	f << indent;
    }
    else if (*p == '@')
      manage_alias(p, f);
    else if (*p != '$')
      f << *p++;
    else if (!strncmp(p, "${comment}", 10))
      manage_comment(p, pp, PhpSettings::isGenerateJavadocStyleComment());
    else if (!strncmp(p, "${description}", 14))
      manage_description(p, pp);
    else if (!strncmp(p, "${final}", 8)) {
      p += 8;
      if (isPhpFinal())
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
    }
    else if (! strncmp(p, "${extends}", 10)) {
      p += 10;

      // extends
	
      sep = " extends ";
      
      for (index = 0; index != sup; index += 1) {
	UmlItem * x = ch[index];
	
	if (x->kind() == aRelation)
	  ((UmlRelation *) x)->generate_extends(sep, f, stereotype);
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
	    ((UmlRelation *) x)->generate_implements(sep, f, stereotype);
	}
      }
    }
    else if (! strncmp(p, "${members}", 10)) {
      p += 10;
  
      // members
      
      if (! an_enum) {
	for (index = 0; index != sup; index += 1) {
	  UmlItem * it = ch[index];
	  
	  switch (it->kind()) {
	  case aClass:
	  case aNcRelation:
	    break;
	  default:
	    if (!((UmlClassItem *) it)->phpDecl().isEmpty())
	      ((UmlClassItem *) it)->generate(f, stereotype, indent, enum_item_rank);
	  }
	}
      
	if (*p == '}')
	  f << indent;
      }
    }
    else if (an_enum && ! strncmp(p, "${items}", 8)) {
      p += 8;
  
      // enums items
      for (index = 0; index != sup; index += 1) {
	UmlItem * it = ch[index];
	
	switch (it->kind()) {
	case aClass:
	case aNcRelation:
	  break;
	default:
	  if (!((UmlClassItem *) it)->phpDecl().isEmpty())
	    ((UmlClassItem *) it)->generate(f, stereotype, indent, enum_item_rank);
	}
      }
      
      if (*p == '}')
	f << indent;
    }
    else
      f << *p++;
  }
}

void UmlClass::generate(QTextOStream &, const QCString &, QCString, int &) {
}

void UmlClass::generate_require_onces(QTextOStream & f, QCString & made) {
  if (!phpDecl().isEmpty()) {
    QVector<UmlItem> ch = children();
    unsigned index;
    const unsigned sup = ch.size();
    
    for (index = 0; index != sup; index += 1) {
      UmlClassMember * m = dynamic_cast<UmlClassMember *>(ch[index]);
      
      if (m != 0)
	m->generate_require_onces(f, made);
    }
  }
}

void UmlClass::generate_require_onces(QTextOStream & f, QCString & made,
				      UmlArtifact * using_art) {
  QCString s;
  
  if (isPhpExternal()) {
    s = phpDecl();
    
    int index;
    
    if ((index = s.find('\n')) == -1)
      // wrong form
      return;
      
      s = s.mid(index + 1) + '\n';
      
      for (;;) {
	if ((index = s.find("${name}")) != -1)
	  s.replace(index, 7, name());
	else if ((index = s.find("${Name}")) != -1)
	  s.replace(index, 7, capitalize(name()));
	else if ((index = s.find("${NAME}")) != -1)
	  s.replace(index, 7, name().upper());
	else if ((index = s.find("${nAME}")) != -1)
	  s.replace(index, 7, name().lower());
	else
	  break;
      }
  }
  else {
    UmlArtifact * art = assocArtifact();
    
    if ((art == 0) || (art == using_art))
      return;

    if (PhpSettings::requireOnceWithPath()) {
      UmlPackage * p = art->package();
      UmlPackage * pack = using_art->package();
      QCString dir;
      
      if (PhpSettings::isRelativePath()) {
	QCString empty;
	
	dir = pack->file_path(empty);
      }
      else if (PhpSettings::isRootRelativePath())
	dir = UmlPackage::rootDir();
	  
      s = "require_once \'" + p->file_path(art->name(), dir) + "';\n";
    }
    else
      s = "require_once \'" + art->name() + '.' +
	PhpSettings::sourceExtension() + "';\n";
  }
  
  if (made.find(s) == -1) {
    made += s;
    if (using_art->phpSource().find(s) == -1)
      f << s;
  }
}

void UmlClass::write(QTextOStream & f, const UmlTypeSpec & t)
{
  if (t.type != 0)
    t.type->write(f);
  else
    f << t.explicit_type;
}

void UmlClass::write(QTextOStream & f) {
  QCString nasp;
  UmlArtifact * a = associatedArtifact();

  if (a != 0)
    // no nested class in Php
    nasp = ((UmlPackage *) a->parent()->parent())->phpNamespace();
  
  if (PhpSettings::isForceNamespacePrefixGeneration()) {
    f << '\\';
    
    if (!nasp.isEmpty())
      f << nasp << '\\';
  }
  else {
    const QCString & currentNasp =
      UmlArtifact::generation_package()->phpNamespace();
    
    if (nasp != currentNasp) {    
      if (nasp.isEmpty())
	f << '\\';
      else if ((strncmp(nasp, currentNasp, currentNasp.length()) == 0) &&
	       (nasp.at(currentNasp.length()) == '\\'))
	f << ((const char *) nasp) + currentNasp.length() + 1 << '\\';
      else
	f << '\\' << nasp << '\\';
    }
  }

  if (isPhpExternal()) {
    QCString s = phpDecl().stripWhiteSpace();
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
  else
    f << name();
}

UmlArtifact * UmlClass::assocArtifact() {
  UmlClass * cl = this;
  
  while (parent()->kind() == aClass)
    cl = (UmlClass *) parent();
  
  return cl->associatedArtifact();
}
