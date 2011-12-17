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

#include "ClassContainer.h"
#include "UmlClass.h"
#include "Class.h"
#include "Lex.h"

ClassContainer::~ClassContainer() {
}

// on exit t.explicit_type is non empty if
// the type is ...<...>... ie *cl and typespec.type are wrong
//
// if first_actual_class is != 0 this means the template class
// is unknown, and first_actual_class is the first known class
// among actuals, def is genericname<...${type}...>

bool ClassContainer::read_type(UmlTypeSpec & typespec, Class ** cl, 
			       const QValueList<FormalParameterList> & tmplts,
			       QValueList<UmlTypeSpec> * actuals,
			       QCString & str_actuals, QCString s,
			       UmlClass ** first_actual_class, QCString & def,
			       QCString & genericname) {
  str_actuals = 0;
  if (actuals != 0)
    actuals->clear();
  
  if (s.isEmpty() && (s = Lex::read_word()).isEmpty()) {
    Lex::premature_eof();
    return FALSE;
  }
    
  QCString path; // type without <..>
  QCString type; // real type form
  bool internal_template = FALSE;	// true if type is ...<...>...
  int pfixdef_length = 0;		// generic form including first class
  int first_actual_class_length = 0;	// first class's name length
  
  genericname = s;
  
  for (;;) {
    internal_template = (path != type);
    
    path += s;
    type += s;
    
    s = Lex::read_word();
  
    if (s != "<")
      break;
    
    type += s;
    str_actuals = s;
    
    // read <...>
    do {
      Lex::mark();
      
      int level = 0;
      QCString firstword;	// first element in current actual
      int pfixlen = 0;		// type length including firstword
      
      for (;;) {
	s = Lex::read_word(TRUE);
	
	if (s == ",") {
	  if (level == 0)
	    break;
	}
	else if (s == ">") {
	  if (level-- == 0)
	    break;
	}
	else if (s == "]")
	  level -= 1;
	else if ((s == "<") || (s == "["))
	  level += 1;
	else if (s.isEmpty()) {
	  Lex::premature_eof();
	  return FALSE;
	}
	else if (firstword.isEmpty()) {
	  firstword = s;
	  pfixlen = type.length() + Lex::region().length();
	}
      }
      
      QCString e = Lex::region();
      
      type += e;
      str_actuals += e;

      if (actuals != 0) {
	UmlTypeSpec t;
	
	e.resize(e.length()); // remove , or >
	e = e.stripWhiteSpace();
	if (! e.isEmpty())
	  compute_type(e, t, tmplts);
	if (actuals != 0)
	  actuals->append(t);
      }
      else if ((first_actual_class != 0) &&
	       (*first_actual_class == 0) && 
	       !firstword.isEmpty()) {
	UmlTypeSpec t;
	
	compute_type(firstword, t, tmplts);
	if (t.type != 0) {
	  *first_actual_class = t.type;
	  first_actual_class_length = firstword.length();
	  pfixdef_length = pfixlen - first_actual_class_length;
	}
      }
    } while (s == ",");
    
    s = Lex::read_word();
    if (s.isEmpty() || (*s != '.'))
      break;
    str_actuals = 0;
    if (actuals != 0)
      actuals->clear();
  }
  
  if (! s.isEmpty())
    Lex::unread_word(s);
  
  compute_type(path, typespec, tmplts, cl);
  if (typespec.type != 0) {
    if (internal_template)
      // typespec.type stay unchanged
      typespec.explicit_type = type;
  }
  else if ((first_actual_class != 0) && (*first_actual_class != 0)) {
    def = type.left(pfixdef_length) + "${type}" + 
      type.mid(pfixdef_length+first_actual_class_length);
  }
  else
    // path may be not good
    typespec.explicit_type = type;
  
  return TRUE;
}

