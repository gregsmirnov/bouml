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

#ifdef DEBUG_BOUML
#include <iostream>

using namespace std;
#endif

#include "Class.h"
#include "CppSettings.h"
#include "Lex.h"
#include "UmlOperation.h"
#include "UmlBaseRelation.h"
#include "Package.h"

#ifdef ROUNDTRIP
#include "UmlArtifact.h"
#endif

QValueList<FormalParameterList> ClassContainer::empty;

// to not have warning
ClassContainer::~ClassContainer() {
}

Class * ClassContainer::declare_if_needed(const QCString & name,
					  const QCString & stereotype,
					  const FormalParameterList & formals,
					  NDict<Class> & declared, 
					  NDict<Class> & defined) {
  if (name.find(' ') != -1)
    // anonymous struct/union/enum
    return 0;
  
  if (name.find('$') != -1)
    // pointer to function/operation
    return 0;
  
  if (! formals.isEmpty()) {
    FormalParameterList::ConstIterator it;
    QCString st = stereotype;
    
    if (st.isEmpty())
      st = "class";
    
    for (it = formals.begin(); it != formals.end(); ++it)
      if (((*it).name() == name) && ((*it).type() == st))
	// template formal
	return 0;
  }
  
  Class * result = 0;
  
  if (((result = declared[name]) == 0) &&
      ((result = defined[name]) == 0) &&
      CppSettings::umlType(name).isEmpty() &&	// not int ...
      CppSettings::include(name).isEmpty() &&	// not external type
      (name != "void") && (name != "unsigned") && (name != "signed")) {
    return new_class(name, stereotype, TRUE);
  }
  
  return ((result != 0) && result->set_stereotype(stereotype))
    ? result : 0;
}

Class * ClassContainer::define(const QCString & name,
			       const QCString & stereotype,
			       NDict<Class> & declared, 
			       NDict<Class> & defined) {
  if (! name.isEmpty()) {
    Class * cl = declared[name];
    
    if (cl != 0) {
      if (! cl->set_stereotype(stereotype))
	return 0;
      defined.insert(name, cl);
      declared.remove(name);
      return cl;
    }
    
    cl = defined[name];
    
    if (!Package::scanning())
      return cl;
    
    if (cl != 0) {
#ifdef ROUNDTRIP
      if (cl->is_roundtrip_expected()) {
	// first step, was never declared, set stereotype
	if (! cl->set_stereotype(stereotype))
	  return 0;
	
	return cl;
      }
      else if ((cl->get_uml() != 0) &&
	       (cl->get_uml()->associatedArtifact() != 0) &&
	       !cl->get_uml()->associatedArtifact()->is_fully_updated())
	// class not roundtriped but defined in the same file of a reverse file
	return cl;
#endif
      Lex::warn("<font color =\"red\"> " + Lex::quote(name) +
		"</font> multiply defined");  
      return 0;    
    }
  }
  
  return new_class(name, stereotype, FALSE);
}

void ClassContainer::compute_type(QCString type, UmlTypeSpec & typespec,
				  QCString & typeform,
				  bool get_first_template_actual,
				  const QValueList<FormalParameterList> & tmplts) {
  typespec.type = 0;
  typespec.explicit_type = 0;
  
  if (!strncmp((const char *) type, "struct ", 7) ||
      !strncmp((const char *) type, "union ", 6) ||
      !strncmp((const char *) type, "enum ", 5)) {
    typespec.explicit_type = "<complex type>";
    typeform = type;
    return;
  }
    
  int index;
  
  if (get_first_template_actual && ((index = type.find('<')) != -1)) {
    type = Lex::normalize(type);
    index = type.find('<');
    
    const char * p = type;
    
    if (strncmp(p + index + 1, "const ", 6) == 0)
      index += 6;
    
    // look at each actual in <>
    unsigned level = 1;
    int index2;
    QCString tf1;
    QCString t1;
    
    for (;;) {
      // earch for the current arg end
      for (index2 = index + 1; p[index2]; index2 += 1) {
	char c = p[index2];
	
	if ((c == ',') || (c == '*') || (c == '[') || (c == '&')) {
	  if (level == 1)
	    break;
	}
	else if (c == '<')
	  level += 1;
	else if ((c == '>') && (--level == 0))
	  break;
      }
      
      if (p[index2]) {
	QCString tf = type.left(index + 1) + typeform + type.mid(index2);
	QCString t = type.mid(index + 1, index2 - index - 1).stripWhiteSpace();
#ifdef DEBUG_BOUML
	cout << "typeform '" << tf << "' type '" << t << "'\n";
#endif
	UmlTypeSpec ts;
	
	QCString normalized = Lex::normalize(t);
  
	if (!find_type(normalized, ts) &&
	     (Namespace::current().isEmpty() ||
	      (normalized.at(0) == ':') || 
	      !find_type("::" + normalized, ts))) {
	  if (get_first_template_actual) {
	    get_first_template_actual = FALSE;
	    tf1 = tf;
	    t1 = t;
	  }
	  index = index2;
	}
	else {
	  // find a class
	  typeform = tf;
	  type = t;
	  typespec.type = ts.type;
	  break;
	}
      }
      else if (!get_first_template_actual) {
	// has first actual
	typeform = tf1;
	type = t1;
	break;
      }
      else {
	typespec.explicit_type = type;
	return;
      }
    }
  }

  if (! tmplts.isEmpty()) {
    QValueList<FormalParameterList>::ConstIterator it1;
    
    for (it1 = tmplts.begin(); it1 != tmplts.end(); ++it1) {
      FormalParameterList::ConstIterator it2;
    
      for (it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
	if ((*it2).name() == type) {
	  typespec.type = 0;
	  typespec.explicit_type = type;
	  return;
	}
      }
    }
  }
  
  QCString normalized;
  
  if (typespec.type == 0) {
    normalized = Lex::normalize(type);
    
    if (!find_type(normalized, typespec) &&
	(Namespace::current().isEmpty() ||
	 (normalized.at(0) == ':') || 
	 !find_type("::" + normalized, typespec))) {
      typespec.explicit_type = CppSettings::umlType(type);
      if (typespec.explicit_type.isEmpty()) {
	// search for equivalent forms
	if (type == "long int")
	  typespec.explicit_type = CppSettings::umlType("long");
	else if (type == "long")
	  typespec.explicit_type = CppSettings::umlType("long int");
	else if (type == "unsigned long int")
	  typespec.explicit_type = CppSettings::umlType("unsigned long");
	else if (type == "unsigned long")
	  typespec.explicit_type = CppSettings::umlType("unsigned long int");
	else if (type == "unsigned")
	  typespec.explicit_type = CppSettings::umlType("unsigned int");
	else if (type == "unsigned int")
	  typespec.explicit_type = CppSettings::umlType("unsigned");
	else if ((type == "signed") || (type == "signed int"))
	  typespec.explicit_type = CppSettings::umlType("int");
      }
      
      if (typespec.explicit_type.isEmpty()) {
	typespec.explicit_type = type; /*
	if (!Lex::identifierp(type, TRUE))
	  typespec.explicit_type = type;
	else {
	  QCString t = type;
	  
	  while ((index = t.find(':')) == 0)
	    t = t.mid(1);
	  
	  ClassContainer * cc = Package::unknown();
	  
	  while (index != -1) {
	    if ((cc = cc->declare_if_needed(t.left(index))) == 0) {
	      typespec.explicit_type = type;
	      return;
	    }
	    
	    while (t[index] == ':')
	      index += 1;
	    t = t.mid(index);
	    index = t.find(':');
	  }
	  
	  if (((cc = cc->declare_if_needed(t)) == 0) ||
	      ((typespec.type = ((Class *) cc)->get_uml()) == 0))
	    typespec.explicit_type = type;
	}*/
      }
      typespec.type = 0;
    }
  }
  
  if ((typespec.type != 0) &&
      !typespec.type->formals().isEmpty() &&
      (type.at(type.length() - 1) == '>') &&
      (!typespec.type->inside_its_definition() ||
       !typespec.type->is_itself((normalized.isEmpty()) ? Lex::normalize(type)
							: normalized))) {
    typespec.type = 0;
    typespec.explicit_type = type;
  }
}

bool ClassContainer::find_type(QCString type, UmlTypeSpec & typespec,
			       NDict<Class> & defined) {
  typespec.explicit_type = 0;
  
  if ((typespec.type = UmlClass::used(type)) != 0)
    return TRUE;
  
  Class * cl = defined[type];
  
  if (cl == 0) {
    int index = 0;
    
    while ((index = type.find('<', index)) != -1) {
      // goes after <...>
      int index2 = index + 1;
      unsigned level = 1;
    
      for (;;) {
	int c = type[index2++];
      
	if (c == '<')
	  level += 1;
	else if (c == 0)
	  // wrong template spec
	  return FALSE;
	else if ((c == '>') && (--level == 0)) {
	  break;
	}
      }
      
      if ((type[index2] != 0) && (defined[type.left(index2)] != 0))
	// explicit template
	index = index2;
      else if (defined[type.left(index)] != 0)
	// non explicit template, remove <>
	type.remove(index, index2 - index);
      else
	// unknown type
	return FALSE;

      typespec.type = 0;
      
      if ((cl = defined[type]) != 0)
	break;
    }
  }
  
  return ((cl != 0) && ((typespec.type = cl->get_uml()) != 0));
}
      
bool ClassContainer::get_template(FormalParameterList & tmplt)
{
  tmplt.clear();
  
  QCString t = Lex::read_word(TRUE);
  
  if (t != "<") {
    if (!Package::scanning() && (t != "class"))
      Lex::syntax_error("&lt; expected after <font color =\"red\"> template</font>");
    UmlOperation::skip_body();
    return FALSE;
  }
  else {
    for (;;) {
      Lex::mark();
      
      t = Lex::read_word();
      
      if (t == ">")
	break;
      
      QCString x;
      QCString s;
      
      if ((t == "class") || (t == "typename")) {
	x = Lex::read_word();
	if (x.isEmpty()) {
	  if (! Package::scanning())
	    Lex::syntax_error("template formal expected");
	  return FALSE;
	}

	s = Lex::read_word(TRUE);
      }
      else if (!Lex::identifierp(t, FALSE)) {
	if (! Package::scanning()) {
	  if (t.isEmpty())
	    Lex::premature_eof();
	  else
	    Lex::error_near(t);
	}
	return FALSE;
      }
      else {
	QCString pre_pre_region;
	QCString pre_region = Lex::region();
	QCString pre_pre_word;
	QCString pre_word = t;
	int level = 0;
	
	for (;;) {
	  s = Lex::read_word(TRUE);
	  if (s.isEmpty()) {
	    if (! Package::scanning())
	      Lex::premature_eof();
	    return FALSE;
	  }
	  if ((level == 0) && ((s == "=") || (s == ",") || (s == ">"))) {
	    if (pre_pre_word.isEmpty()) {
	      // only one form
	      if (! Package::scanning())
		Lex::error_near(s);
	      return FALSE;
	    }
	    if ((pre_pre_word == "class") && Lex::identifierp(pre_word, TRUE)) {
	      t = pre_pre_region;
	      x = pre_word;
	    }
	    else {
	      t = "";
	      x = pre_region;
	    }
	    break;
	  }
	  pre_pre_region = pre_region;
	  pre_region = Lex::region();
	  pre_pre_word = pre_word;
	  pre_word = s;
	  if ((s == "(") || (s == "[") || (s == "<"))
	    level += 1;
	  else if ((s == ")") || (s == "]") || (s == ">"))
	    level -= 1;
	}	
      }
      
      QCString v;
		
      if (s == "=") {
	v = Lex::read_list_elt();
	s = Lex::read_word(TRUE);
      }
	
      tmplt.append(UmlFormalParameter(t, x, v));
      
      if (s == ">")
	break;
      if (s != ",") {
	if (! Package::scanning())
	  Lex::error_near(s);
	return FALSE;
      }
    }
    
    return TRUE;
  }
}

