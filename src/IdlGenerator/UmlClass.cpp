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

#include <stdio.h>
#include <qtextstream.h>

#include "UmlClass.h"
#include "UmlRelation.h"
#include "IdlSettings.h"
#include "UmlArtifact.h"
#include "UmlPackage.h"
#include "UmlCom.h"
#include "util.h"

QCString UmlClass::idl_stereotype()
{
  QCString s = IdlSettings::classStereotype(stereotype());
  
  return ((s == "struct") || (s == "union") || (s == "enum") ||
	  (s == "typedef") || (s == "exception") || (s == "ignored") ||
	  (s == "interface"))
    ? s : QCString("valuetype");
}

void UmlClass::generate() {
  if (! managed) {
    managed = TRUE;
    
    if (! isIdlExternal()) {
      if (associatedArtifact() != 0)
	associatedArtifact()->generate();
      else if (verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica><i> ") + name() +
		      " : </i> does not have associated <i>artifact</i></font>");
    }
  }
}

void UmlClass::generate(QTextOStream & f) {
  if (idlDecl().isEmpty())
    return;
  
  QVector<UmlItem> ch = children();
  QCString stereotype = idl_stereotype();
  bool a_typedef = (stereotype == "typedef");
  bool an_enum = (stereotype == "enum");
  bool an_union = (stereotype == "union");
  const char * p = idlDecl();
  const char * pp = 0;
  QCString templ;
  const char * sep;
  QCString indent = "";
  unsigned index;
  
  while ((*p == ' ') || (*p == '\t'))
    indent += *p++;
  
  if (*p != '#')
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
      if (*p != '#')
	f << indent;
    }
    
    if (*p == '\n') {
      f << *p++;
      if (*p && (*p != '#') &&
	  strncmp(p, "${items}", 8) &&
	  strncmp(p, "${members}", 10))
	f << indent;
    }
    else if (*p == '@')
      manage_alias(p, f);
    else if (*p != '$')
      f << *p++;
    else if (!strncmp(p, "${comment}", 10))
      manage_comment(p, pp);
    else if (!strncmp(p, "${description}", 14))
      manage_description(p, pp);
    else if (!strncmp(p, "${abstract}", 11)) {
      p += 11;
      if (isAbstract())
	f << "abstract ";
    }
    else if (!strncmp(p, "${local}", 8)) {
      p += 8;
      if (isIdlLocal())
	f << "local ";
    }
    else if (!strncmp(p, "${custom}", 9)) {
      p += 9;
      if (isIdlCustom())
	f << "custom ";
    }
    else if (!strncmp(p, "${name}", 7)) {
      p += 7;
      f << name();
    }
    else if (a_typedef) {
      if (!strncmp(p, "${type}", 7)) {
	p += 7;
	f << baseType().toString();
	
	UmlClass * cl = baseType().type;
	const QValueList<UmlActualParameter> & actuals = this->actuals();
	
	if ((cl != 0) && !actuals.isEmpty()) {
	  QValueList<UmlActualParameter>::ConstIterator it;
	  
	  for (it = actuals.begin(); it != actuals.end(); ++it)
	    if ((*it).superClass() == cl)
	      (*it).generate(f);
	  
	  f << '>';
	}
      }
      else
	// strange
	f << *p++;
    }
    else if (an_enum) {
      if (!strncmp(p, "${items}", 8)) {
	p += 8;
	
	// items declaration
	
	unsigned n = 0;
	
	for (index = 0; index != ch.size(); index += 1)
	  if ((ch[index]->kind() != aNcRelation) &&
	      !((UmlClassItem *) ch[index])->idlDecl().isEmpty())
	    n += 1;
	
	for (index = 0; n != 0; index += 1)
	  if ((ch[index]->kind() != aNcRelation) &&
	      !((UmlClassItem *) ch[index])->idlDecl().isEmpty())
	    ((UmlClassItem *) ch[index])->generate_decl(f, stereotype, --n == 0);
      
	if (*p == '}')
	  f << indent;
      }
      else
	// strange
	f << *p++;
    }
    else if (! strncmp(p, "${inherit}", 10)) {
      p += 10;
      
      // inherit
      
      sep = " : ";
      BooL already = FALSE;
      
      for (index = 0; index != ch.size(); index += 1) {
	UmlItem * x = ch[index];
	
	if ((x->kind() == aRelation) &&
	    !((UmlRelation *) ch[index])->idlDecl().isEmpty())
	  ((UmlRelation *) x)->generate_inherit(sep, f, stereotype, already);
      }
    }
    else if (! strncmp(p, "${members}", 10)) {
      p += 10;
      
      // members declaration
      
      for (index = 0; index != ch.size(); index += 1)
	if ((ch[index]->kind() != aNcRelation) &&
	    !((UmlClassItem *) ch[index])->idlDecl().isEmpty())
	  ((UmlClassItem *) ch[index])->generate_decl(f, stereotype, indent);
      
      if (*p == '}')
	f << indent;
    }
    else if (!strncmp(p, "${switch}", 9) && an_union) {
      p += 9;
      f << switchType().toString();
    }
    else
      // strange
      f << *p++;
  }
}

void UmlClass::generate_decl(QTextOStream &, const QCString &, 
			     QCString, bool) {
  write_trace_header();
  UmlCom::trace(QCString("<font color=\"red\"><b>Embedded class <it>")
		+ name() + "</it> not generated</b></font><br>");
  incr_warning();
}

void UmlClass::write(QTextOStream & f, const UmlTypeSpec & t)
{
  if (t.type != 0)
    t.type->write(f);
  else
    f << IdlSettings::type(t.explicit_type);
}

void UmlClass::write(QTextOStream & f) {
  UmlItem * p = associatedArtifact();
  
  if (p == 0)
    p = this;
  
  do {
    p = p->parent();
  } while (p->kind() != aPackage);
  
  QCString module = ((UmlPackage *) p)->idlModule();
  
  if (module != UmlArtifact::generation_package()->idlModule())
    f << module << "::";
  
  if (isIdlExternal()) {
    QCString s = idlDecl();
    int index = s.find('\n');
    
    s = (index == -1) ? s.stripWhiteSpace()
		      : s.left(index).stripWhiteSpace();
    
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
    f << name();	// true_name
}
