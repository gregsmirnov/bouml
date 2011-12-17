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

#ifdef TRACE
#include <iostream>
using namespace std;
#endif

#include <qstringlist.h> 

#include "UmlOperation.h"
#include "Package.h"
#include "UmlPackage.h"
#include "Class.h"
#include "UmlClass.h"
#include "UmlAttribute.h"
#include "PhpCatWindow.h"
#include "PhpSettings.h"
#include "Lex.h"
#ifdef REVERSE
#include "Statistic.h"
#endif

// not in scanning

bool UmlOperation::new_one(Class * container, aVisibility visibility,
			   bool finalp, bool abstractp, bool staticp,
			   QCString comment, QCString description)
{
  // 'function' was read, it is followed by :
  // ['&'] name'(' {'array' | <classname>] ['&'] '$'<varname> ['=' <value>]}* ')' '{' ... '}'
  QCString s = Lex::read_word();
  bool refp;
  
  if (s == "&") {
    refp = TRUE;
    s = Lex::read_word();
  }
  else
    refp = FALSE;
  
  if (s.isEmpty()) {
    Lex::premature_eof();
    return FALSE;
  }
    
  QCString name = s;
  
#ifdef TRACE
  cout << "OPERATION '" << name << "'\n";
#endif
  
  s = Lex::read_word();
  if (s != "(") {
    Lex::syntax_error("'(' expected rather than '" + s + "'");
    return FALSE;
  }
  
  UmlClass * cl = container->get_uml();
  UmlOperation * op;
  
#ifndef REVERSE
  if (visibility == PrivateVisibility)
    op = 0;
  else
#endif
  {
    op = UmlBaseOperation::create(cl, name);
    
    if (op == 0) {
      PhpCatWindow::trace(QCString("<font face=helvetica><b>cannot add operation <i>")
			   + name + "</i> in <i>" + cl->name() 
			   + "</i></b></font><br>");  
      return FALSE;
    }
    
#ifdef REVERSE
    Statistic::one_operation_more();
#endif
  }
    
  QCString def;
    
  if (op != 0) {
    op->set_Visibility(visibility);
    if (staticp) op->set_isClassMember(TRUE);
    if (finalp) op->set_isPhpFinal(TRUE);
  
    def = PhpSettings::operationDef();
  
    int index;
    
    if (((index = def.find("${(}")) == -1) ||
	(def.find("${)}", index + 4) == -1) ||
	((index = def.find("${name}")) == -1) ||
	(def.find("${body}") == -1)) {
      // use a definition where ${body] is not indented
      def = "  ${comment}${final}${visibility}${abstract}${static}function ${name}${(}${)}\n{\n  ${body}}\n";
      index = def.find("${name}");
    }
    
    if (refp)
      def.insert(index, "&");
    
    if ((name == cl->name()) || (name == "__construct")) {
      // constructor, remove useless ${}
      if ((index = def.find("${static}")) != -1)
	def.remove(index, 9);
      if ((index = def.find("${final}")) != -1)
	def.remove(index, 8);
      if ((index = def.find("${abstract}")) != -1)
	def.remove(index, 11);
    }
    
    if (abstractp) {
      op->set_isAbstract(TRUE);
      
      def = def.left(def.find("${)}") + 4) + ";";
    }
  }
  
  // parameters
  
  unsigned rank = 0;
  UmlParameter param;
  
  while (read_param(container, rank, param, def, op == 0)) {
    if ((op != 0) && ! op->addParameter(rank++, param)) {
      PhpCatWindow::trace(QCString("<font face=helvetica><b>cannot add param <i>")
			   + name + "</i> in <i>" + cl->name() 
			   + "</i></b></font><br>");  
#ifdef TRACE
      cout << "ERROR cannot add param '" << param.name << '\n';
#endif
      return FALSE;
    }
  }
  
  s = Lex::read_word();
  
  if (s.isEmpty()) {
    Lex::premature_eof();
    return FALSE;
  }
    
  // definition
  
  if (abstractp || (cl->stereotype() == "interface")) {
    if (s != ";") {
      Lex::error_near(s);
      return FALSE;
    }
#ifdef REVERSE
    if (op != 0)
      op->set_PhpBody(0);
#endif
  }
  else if (s != "{") {
    Lex::error_near(s);
    return FALSE;
  }
  else {
    Lex::mark();
    
    // goto the end of the body
    
    char c;
    int level = 1;	// '{' already read
    
    for (;;) {
      if ((c = Lex::read_word_bis()) == 0)
	return FALSE;
      else if (c == '{')
	level += 1;
      else if ((c == '}') && (--level == 0))
	break;
    }
    
#ifdef REVERSE
    if (op != 0) {
      QCString e = Lex::region();
      
      e.truncate(e.length() - 1);	// remove }

      // remove fist \n
      if (*((const char *) e) == '\n')
	e.remove(0, 1);
      
      // remove last spaces and tabs
      int ln = e.length();
      
      while (ln && ((e[ln - 1] == ' ') || (e[ln - 1] == '\t')))
	ln -= 1;
      e.truncate(ln);
      
      op->set_PhpBody(e);
      op->set_PhpContextualBodyIndent(FALSE);
    }
#endif
  }
  
  if ((op != 0) && (def != op->phpDecl()))
    op->set_PhpDecl(def);
  
  Lex::clear_comments();	// params & body comments
  Lex::finish_line();
  
  if ((op != 0) && !comment.isEmpty()) {
    s = (def.find("${description}") != -1) ? description : comment;
    
    UmlTypeSpec t;
    int index1;
    
    if (! (t.explicit_type = value_of(s, "@return", index1)).isEmpty()) {
      op->set_ReturnType(t);
      s.replace(index1, t.explicit_type.length(), "${type}");
    }
    
    QValueList<UmlParameter> l = op->params();
    unsigned nparams = l.count();

    if (nparams != 0) {
      QCString varname;
      int index2;
      char xn[16];

      index1 = 0;
      rank = 0;
      
      while (!((t.explicit_type = value_of(s, "@param", index1, varname, index2))
	       .isEmpty())) {
	if (varname.isEmpty() || (varname[0] != '$')) {
	  if (rank < nparams) {
	    UmlParameter & p = l[rank];
	    
	    p.type = t;
	    op->replaceParameter(rank, p);
	  }
	}
	else {
	  varname = varname.mid(1);
	  
	  QValueList<UmlParameter>::Iterator it;
	  
	  for (it = l.begin(), rank = 0; it != l.end(); ++it, rank += 1) {
	    if ((*it).name == varname) {
	      (*it).type = t;
	      op->replaceParameter(rank, *it);
	      sprintf(xn, "${p%d}", rank);
	      s.replace(index2, varname.length() + 1, xn);
	      break;
	    }
	  }
	}
	sprintf(xn, "${t%d}", rank++);
	s.replace(index1, t.explicit_type.length(), xn);
      }
    }
    op->set_Description(s);
  }
  
  return TRUE;
}

// a param is : {'array' | <classname>] ['&'] '$'<varname> ['=' <value>]

bool UmlOperation::read_param(Class * container, unsigned rank,
			      UmlParameter & param, QCString & def, bool bypass)
{
#ifdef TRACE
  cout << "UmlOperation::manage_param " << rank << "\n";
#endif
  
  QCString s = Lex::read_word();
  
  if (s.isEmpty()) {
    Lex::premature_eof();
    return FALSE;
  }
  else if (s == ")")
    return FALSE;
  
#ifdef TRACE
  cout << "commence par " << s << '\n';
#endif

  param.type.type = 0;
  param.type.explicit_type = 0;
  param.dir = InputOutputDirection;

  bool arrayp = FALSE;
  
  if (s == "array") {
    arrayp = TRUE;
    s = Lex::read_word();
    if (s.isEmpty()) {
      Lex::premature_eof();
      return FALSE;
    }
  }
  else if ((((const char *) s)[0] != '&') &&
	   (((const char *) s)[0] != '$')) {
    // a type
    container->compute_type(s, param.type, 0);
    
    s = Lex::read_word();
    if (s.isEmpty()) {
      Lex::premature_eof();
      return FALSE;
    }
  }
  
  bool refp;
  
  if (s == "&") {
    refp = TRUE;
    s = Lex::read_word();
    if (s.isEmpty()) {
      Lex::premature_eof();
      return FALSE;
    }
  }
  else
    refp = FALSE;
  
  if (((const char *) s)[0] != '$') {
    Lex::syntax_error("invalid parameter name : " + s);
    return FALSE;
  }
  
  if (! bypass) {
    QCString n_close = QCString().setNum(rank) + "}";
    
    param.name = s.mid(1);
	
    s = (rank == 0) ? "" : ", ";
    
    if (arrayp)
      s += "array ";
    else if ((param.type.type != 0) || !param.type.explicit_type.isEmpty()) {
      s += "${t" + n_close + " ";
    }
    
    if (refp)
      s += "& ";

    s += "${p" + n_close + "${v" + n_close;
    
    def.insert(def.find("${)}"), 	// cannot be -1
	       s);
  }
  
  s = Lex::read_word();
  
  if (s == "=") {
    Lex::mark();
    s = skip_expr(0);
    param.default_value = Lex::region();
    param.default_value.truncate(param.default_value.length() - s.length());
    if (*((const char *) param.default_value) == ' ')
      param.default_value = param.default_value.mid(1);
  }    
  else if (s.isEmpty()) {
    Lex::premature_eof();
    return FALSE;
  }
  else
    param.default_value = 0;
  
  if (s == ")")
    Lex::unread_word(s);
  
  return TRUE;
}
	 
void UmlOperation::skip_body(int level) {
  char c;
    
  while ((c = Lex::read_word_bis()) != 0) {	// not end of file
    if (c == '}') {
      if (--level == 0)
	// end of body
	break;
    }
    else if (c == ';') {
      if (level == 0)
	// no body
	break;
    }
    else if (c == '{')
      level += 1;
  }
  
  Lex::clear_comments();
}
	 
QCString UmlOperation::skip_expr(int level) {
  char c;
    
  while (((c = Lex::read_word_bis()) != 0) &&
	 ((c != ';') || (level != 0)) &&
	 ((c != ',') || (level != 0))) {
    if ((c == '{') || (c == '(') || (c == '['))
      level += 1;
    else if ((c == '}') || (c == ')') || (c == ']')) {
      if (level == 0)
	break;
      
      level -= 1;
    }
  }
  
  Lex::clear_comments();
  
  QCString e;
  
  if (c != 0)
    e += c;
  
  return e;
}
