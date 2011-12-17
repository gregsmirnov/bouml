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

bool ClassContainer::read_type(UmlTypeSpec & typespec, Class ** cl) {
  QCString s = Lex::read_word();
  
  if (s.isEmpty()) {
    Lex::premature_eof();
    return FALSE;
  }
    
  compute_type(s, typespec, cl);
  
  return TRUE;
}

// don't produce error

bool ClassContainer::bypass_type(QCString s) {
  if (s.isEmpty() && (s = Lex::read_word()).isEmpty())
    return FALSE;
    
  for (;;) {
    s = Lex::read_word();
  
    if (s != "<")
      break;
    
    do {
      int level = 0;
      
      for (;;) {
	s = Lex::read_word();
	
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
	else if (s.isEmpty())
	  return FALSE;
      }
    } while (s == ",");
    
    s = Lex::read_word();
    if (s.isEmpty() || (*s != '.'))
      break;
  }
  
  if (! s.isEmpty())
    Lex::unread_word(s);
  
  return TRUE;
}
