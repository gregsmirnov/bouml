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

#include <qstringlist.h> 

#include "UmlOperation.h"
#include "Package.h"
#include "UmlClass.h"
#include "Class.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "Lex.h"
#ifdef REVERSE
#include "Statistic.h"
# ifdef ROUNDTRIP
# include "UmlCom.h"
# endif
#endif

NDict< QList<UmlOperation> > UmlOperation::friends;
#ifdef ROUNDTRIP
QPtrDict<QCString> UmlOperation::DefNotYetSet(97);
#endif

UmlOperation::UmlOperation(void * id, const QCString & n)
    : UmlBaseOperation(id, n), formals(0) {
}

UmlOperation::~UmlOperation() {
  if (formals != 0)
    delete formals;
}
    
bool UmlOperation::pfunc(BooL & func, QCString & name, QCString & type,
			 const char * namespec)
{
  // Lex:mark before type, first '(' read
  QCString s;
  const LexContext here = Lex::get_context();
  
  while (!Lex::star(s = Lex::read_word())) {
    if (s.isEmpty() || (s == "}") || (s == ")") || (s == ";")) {
      Lex::set_context(here);
      Lex::error_near("(");
      return FALSE;
    }
  }

  do name = Lex::read_word();
  while (Lex::star(name));
  
  if (name == ")") {
    // no var name
    Lex::unread_word();
    name = 0;
  }
    
  type = Lex::region();
  type.truncate(type.length() - name.length()); // remove the name
  name = Lex::complete_template_type(name);
  type += namespec;
  
  Lex::mark();
  
  for (;;) {
    switch (Lex::read_word_bis(TRUE, FALSE)) {
    case 0:
      Lex::come_back();
      Lex::error_near("(");
      return FALSE;
    case ')':
      // not a function/operation returning a function/operation
      if ((s = Lex::read_word()) != "(") {
	Lex::error_near(s);
	return FALSE;
      }
      
      skip_expr(")", TRUE);
      
      func = FALSE;
      type += Lex::region();	// complete
      return TRUE;
    case '(':
      // a function/operation returning a function/operation
      func = TRUE;
      type += Lex::region();	// not complete
      type.truncate(type.length() - 1); // remove '('
      return TRUE;
    }
  }
}

static void insert_template(const FormalParameterList & tmplt, 
			    QCString & s, bool decl)
{
  // computes "template<....>"
  QCString tm;
  FormalParameterList::ConstIterator it;
  const char * sep = "template<";
  
  for (it = tmplt.begin(); it != tmplt.end(); ++it) {
    tm += sep + (*it).type() + ' ' + (*it).name();
    
    const QCString & t = (*it).defaultValue().toString();
    
    if (! t.isEmpty())
      tm += " = " + t;
  }
  
  tm += (decl) ? ">\n  " : ">\n";
  
  // search the beginning of the definition/declaration in s;
  const char * ps  = s;
  const char * p = ps;
  
  for (;;) {
    if (*p == 0)
      return;
    
    if (*p <= ' ')
      p += 1;
    else if (*p == '/') {
      if (p[1] == '/') {
	// comment
	p += 1;
	do p += 1; 
	while (*p && (*p != '\n'));
      }
      else if (p[1] == '*') {
	/* comment */
	p += 1;
	do p += 1; 
	while (*p && ((*p != '*') || (p[1] != '/')));
	p += 2;
      }
      else
	break;
    }
    else if (*p == '#') {
      do {
	if (*++p == '\\')
	  p += 2;
	else if ((*p == '/') && (p[1] == '*')) {
	  /* comment */
	  p += 1;
	  do p += 1; while (*p && ((*p != '*') || (p[1] != '/')));
	  p += 2;
	}
      } while (*p && (*p != '\n'));
    }
    else
      break;
  }
  
  if (s.mid(p - ps, 10) == "${comment}")
    p += 10;
  else if (s.mid(p - ps, 14) == "${description}")
    p += 14;
  
  // place the template<...> list
  s.insert(p - ps, tm);
}

bool UmlOperation::new_one(Class * cl, const QCString & name,
			   const QValueList<FormalParameterList> & tmplts,
			   QCString type, const QCString & modifier,
			   const QCString & pretype, aVisibility visibility,
			   bool inlinep, bool virtualp, bool staticp, bool constp,
			   bool volatilep, bool typenamep, bool explicitp,
			   bool friendp, QCString friend_template,
			   QCString comment, QCString description, bool pfunc
#ifdef ROUNDTRIP
			   , bool roundtrip, QList<UmlItem> & expected_order
#endif
			   )
{
  // the "(" was read
  
#ifdef DEBUG_BOUML
  cout << "OPERATION '" << name << "' type '" << type << "' modifier '" << modifier << "'\n";
#endif
  
  QCString cl_name = QCString(cl->text(0));
  UmlOperation * op;
#ifdef ROUNDTRIP
  bool may_roundtrip = roundtrip &&
    (!cl->from_libp() || (visibility != PrivateVisibility));
  QValueList<UmlParameter> params;
  QCString body;
  
  if (may_roundtrip)
#else
  if (
# ifdef REVERSE
      cl->from_libp() &&
# endif
      (visibility == PrivateVisibility))
#endif
    op = 0;
  else {
    op = UmlBaseOperation::create(cl->get_uml(), name);
    
    if (op == 0) {
      UmlCom::trace(QCString("<font face=helvetica><b>cannot add operation <i>")
		    + Lex::quote(name) + "</i> in <i>" + Lex::quote(cl_name)
		    + "</i></b></font><br><hr><br>");  
      return FALSE;
    }
    
#ifndef ROUNDTRIP
# ifdef REVERSE
    Statistic::one_operation_more();
# endif
#endif
  }
    
  UmlTypeSpec return_type;
  QCString typeform;
  
  if (!pfunc) {
    typeform = (pretype.isEmpty())
      ? QCString("${type}")
      : pretype + " ${type}";
    
    if (! type.isEmpty())	
      // not a contructor or destructor
      cl->compute_type(type, return_type, typeform, FALSE, tmplts);
  }
  
  int index;
  QCString decl;
  
  if (op != 0) {
    op->set_Visibility(visibility);
    if (staticp) op->set_isClassMember(staticp);
    if (virtualp) op->set_isCppVirtual(virtualp);
    if (inlinep) op->set_isCppInline(inlinep);
    if (friendp) op->set_isCppFriend(friendp);
#ifdef ROUNDTRIP
  }
  if (may_roundtrip || (op != 0)) {
#endif
  
    decl = CppSettings::operationDecl();
    
    if ((decl.findRev(';') == -1) ||
	(decl.find("${friend}") == -1) ||
	(decl.find("${static}") == -1) ||
	((index = decl.find("${(}")) == -1) ||
	((index = decl.find("${)}", index + 4)) == -1) ||
	(decl.find("${volatile}", index + 4) == -1) ||
	(decl.find("${const}", index + 4) == -1) ||
	((index = decl.find("${type}")) == -1) ||
	(decl.find("${name}", index + 7) == -1) ||
	(decl.find("${abstract}", index + 7) == -1)) {
      // throw is not mandatory
      decl = "    ${comment}${friend}${static}${inline}${virtual}${type} ${name}${(}${)}${const}${volatile}${abstract};";
      index = decl.find("${type}");
    }
    
    if (! pfunc) {
      if (!modifier.isEmpty())
	decl.insert(index + 7, QCString(" ") + modifier);
      decl.replace(index, 7, typeform);
      if (typenamep) {
	decl.insert(index, "typename ");
	index += 9;
      }
      if (constp) {
	decl.insert(index, "const ");
	index += 6;
      }
      if (volatilep)
	decl.insert(index, "volatile ");
    }
    else {
      if (typenamep) {
	decl.insert(index, "typename ");
	index += 9;
      }
      if (constp) {
	decl.insert(index, "const ");
	index += 6;
      }
      if (volatilep) {
	decl.insert(index, "volatile ");
	index += 9;
      }
      decl.replace(index, decl.find("${name}") + 7 - index, type);
    }
    
    if (!comment.isEmpty())
      if (op != 0) 
	op->set_Description((decl.find("${description}") != -1)
			    ? description : Lex::simplify_comment(comment));
  
  }
  
  // parameters
  
  unsigned rank = 0;
  UmlParameter param;
  BooL on_error;
  
#ifdef ROUNDTRIP
  // look if operation already exist
  // note : don't use preserve_body identifier like in Java
  // else need to report existing operation detection when
  // will read its body rather than only its declaration
  if (may_roundtrip) {
    while (read_param(cl, rank, param, decl, tmplts, on_error, TRUE)) {
      if (may_roundtrip) {
	params.append(param);
	rank += 1;
      }
    }
  
    if ((op = already_exist(cl, name, params, FALSE)) != 0) {
      // update already existing operation
      op->set_usefull();

      op->update_params(cl, params);

      if (op->visibility() != visibility) {
	cl->set_updated();
	op->set_Visibility(visibility);
      }
    
      if (op->isClassMember() != staticp) {
	cl->set_updated();
	op->set_isClassMember(staticp);
      }

      if (op->isCppVirtual() != virtualp) {
	cl->set_updated();
	op->set_isCppVirtual(virtualp);
      }

      if (op->isCppFriend() != friendp) {
	cl->set_updated();
	op->set_isCppFriend(friendp);
      }
      
      if (decl.find("${description}") != -1) {
	if (nequal(op->description(), description)) {
	  op->set_Description(description);
	  cl->set_updated();
	}
      }
      else if (nequal(op->description(), Lex::simplify_comment(comment))) {
	op->set_Description(comment); // comment was set
	cl->set_updated();
      }
    }
    else {
      // operation doesn't yet exist
      op = UmlBaseOperation::create(cl->get_uml(), name);
      
      if (op == 0) {
	UmlCom::trace(QCString("<font face=helvetica><b>cannot add operation <i>")
		      + Lex::quote(name) + "</i> in <i>" + Lex::quote(cl_name)
		      + "</i></b></font><br><hr><br>");  
	return FALSE;
      }
      
      may_roundtrip = FALSE;
      cl->set_updated();

      op->set_Visibility(visibility);
      if (staticp) op->set_isClassMember(staticp);
      if (virtualp) op->set_isCppVirtual(virtualp);
      if (friendp) op->set_isCppFriend(friendp);
      
      if (!comment.isEmpty())
	op->set_Description((decl.find("${description}") != -1)
			    ? description : Lex::simplify_comment(comment));
      
      QValueList<UmlParameter>::Iterator it;
  
      for (rank = 0, it = params.begin(); it != params.end(); ++it, rank += 1) {
	if (!op->addParameter(rank, *it)) {
# ifdef DEBUG_BOUML
	  cout << "ERROR cannot add param '" << param.name << "' type '" << param.type.toString() << '\n';
# endif
	  return FALSE;
	}
      }
    }
    
    expected_order.append(op);
  }
  else
#endif
    while (read_param(cl, rank, param, decl, tmplts, on_error, TRUE)) {
      if ((op != 0) && !op->addParameter(rank++, param)) {
# ifdef DEBUG_BOUML
	cout << "ERROR cannot add param '" << param.name << "' type '" << param.type.toString() << '\n';
# endif
	return FALSE;
      }
    }
  
  if ((op != 0) && friendp) {
    // not an operation
    QList<UmlOperation> * fr = friends[name];
    
    if (fr == 0) {
      fr = new QList<UmlOperation>;
      fr->append(op);
      friends.insert(name, fr);
    }
    else
      fr->append(op);
  }
  
  QCString s;
  
  if (pfunc) {
    Lex::mark();
  
    if (((s = Lex::read_word()) != ")") ||
	((s = Lex::read_word()) != "(")) {
      Lex::error_near(s);
      return FALSE;
    }
    skip_expr(")");
    decl.insert(decl.find("${)}") + 4, Lex::region());
    if (op != 0) {
      return_type.explicit_type = type + Lex::region();
      return_type.explicit_type.remove(return_type.explicit_type.find("${name}"), 7);
    }
  }
  
  if (op != 0) {
#ifdef ROUNDTRIP
    if (may_roundtrip) {
      if (!op->returnType().equal(return_type)) {
	cl->set_updated();
	op->set_ReturnType(return_type);
      }
    }
    else
#endif
      op->set_ReturnType(return_type);
  }
        
  // const may be before volatile
  bool is_const;
  
  if ((s = Lex::read_word()) == "const") {
    is_const = TRUE;
    s = Lex::read_word();
  }
  else
    is_const = FALSE;
  
  // volatile
  if (s == "volatile") {
    if (op != 0) {
#ifdef ROUNDTRIP
      if (may_roundtrip) {
	if (!op->isVolatile()) {
	  cl->set_updated();
	  op->set_isVolatile(TRUE);
	}
      }
      else
#endif
	op->set_isVolatile(TRUE);
    }
    s = Lex::read_word();
  }
#ifdef ROUNDTRIP
  else if (may_roundtrip && op->isVolatile()) {
    cl->set_updated();
    op->set_isVolatile(FALSE);
  }
#endif
  
  // const may be after volatile
  if (s == "const") {
    is_const = TRUE;
    s = Lex::read_word();
  }
  
  if (op != 0) {
#ifdef ROUNDTRIP
    if (may_roundtrip) {
      if (op->isCppConst() != is_const) {
	cl->set_updated();
	op->set_isCppConst(is_const);
      }
    }
    else
#endif
      if (is_const)
	op->set_isCppConst(TRUE);
  }
  
  // throw
  
  if (s == "throw") {
    if ((s = Lex::read_word()) != "(") {
      Lex::error_near("throw");
      skip_body((s == "{") ? 1 : 0);
      return FALSE;
    }
    
    rank = 0;
    
    if (op == 0)
      skip_expr(")");
    else {
      UmlTypeSpec excpt;
      
#ifdef ROUNDTRIP
      if (may_roundtrip) {
	const QValueList<UmlTypeSpec> old_exceptions = op->exceptions();
	QValueList<UmlTypeSpec>::ConstIterator it_old;
	bool has;
	
	for (it_old = old_exceptions.begin();
	     ((has = read_throw_elt(cl, excpt, tmplts)) != FALSE) &&
	     (it_old != old_exceptions.end());
	     ++it_old, rank += 1) {
	  if (!excpt.equal(*it_old)) {
	    op->replaceException(rank, excpt);
	    cl->set_updated();
	  }
	}
	    
	if (has) {
	  // have missing exceptions
	  cl->set_updated();
	  do {
	    op->addException(rank++, excpt);
	  } while (read_throw_elt(cl, excpt, tmplts));
	}
	else if (it_old != old_exceptions.end()) {
	  // have extra exceptions
	  cl->set_updated();
	  do {
	    op->removeException(rank);
	    it_old++;
	  } while (it_old != old_exceptions.end());
	}
      }
      else
#endif
	while (read_throw_elt(cl, excpt, tmplts))
	  op->addException(rank++, excpt);
    }
    
    if ((rank != 0) && (decl.find("${throw}") == -1))
      decl.insert(decl.find("${abstract}"), "${throw}");
    
    s = Lex::read_word();
  }
  
  if ((index = cl_name.find('<')) != -1)
    cl_name.truncate(index);
  
  if ((name == cl_name) || (name == (QCString("~") + cl_name))) {
    // constructor or destructor : remove useless ${}
    decl.remove(decl.find("${friend}"), 9);
    decl.remove(decl.find("${static}"), 9);
    index = decl.find("${type}");
    decl.remove(index, (((const char *) decl)[index + 7] == ' ') ? 8 : 7);
    decl.remove(decl.find("${const}"), 8);
    decl.remove(decl.find("${abstract}"), 11);
    if ((*((const char *) name) != '~') && ((index = decl.find("${virtual}")) != -1))
      decl.remove(index, 10);
    if (explicitp)
      decl.insert(decl.find("${name}"), "explicit ");
  }
  else if (type.isEmpty() && (name.left(9) == "operator ")) {
    index = decl.find("${type}");
    decl.remove(index, (((const char *) decl)[index + 7] == ' ') ? 8 : 7);
  }
  
  bool defined_in_decl = FALSE;
  
  // abstract
  if (s == "=") {
    if (Lex::read_word() != "0")
      return FALSE;

    if (op != 0) {
#ifdef ROUNDTRIP
      if (may_roundtrip) {
	if (!op->isAbstract()) {
	  cl->set_updated();
	  op->set_isAbstract(TRUE);
	}
      }
      else
#endif
	op->set_isAbstract(TRUE);
    }
    
    s = Lex::read_word();
  }
  else {
#ifdef ROUNDTRIP
    if (may_roundtrip && op->isAbstract()) {
      cl->set_updated();
      op->set_isAbstract(FALSE);
    }
#endif
    // definition
    if ((s == "{") || (s == ":")) {
      defined_in_decl = TRUE;
      if (op != 0) {
#ifdef ROUNDTRIP
	if (may_roundtrip)  {
	  if (! op->cppDef().isEmpty()) {
	    cl->set_updated();
	    op->set_CppDef(0);
	  }
	  if (! op->isCppInline()) {
	    cl->set_updated();
	    op->set_isCppInline(TRUE);
	  }
	}
	else {
#endif
	  op->set_CppDef(0);
	  op->set_isCppInline(TRUE);
#ifdef ROUNDTRIP
	}
#endif
      }
      
      Lex::mark();
      
      // goto the end of the body
      
      char e;
      
      if (s == ":")
	while ((e = Lex::read_word_bis(TRUE, TRUE)) != '{')
	  if (e == 0)
	    return FALSE;
      
      int level = 1;	// '{' already read
      
      for (;;) {
	if ((e = Lex::read_word_bis(TRUE, TRUE)) == 0)
	  return FALSE;
	else if (e == '{')
	  level += 1;
	else if ((e == '}') && (--level == 0))
	  break;
      }
      
      s.insert(0, ' ');
      decl.insert(decl.findRev(';'), s + Lex::region());	// decl contains ';'  
      s = ";";
    }
    else
#ifdef ROUNDTRIP
    // if may_roundtrip don't update inline here because inline def
    // may follow class decl and def set later too
      if (!may_roundtrip)
#endif
	op->set_isCppInline(inlinep);
  }

  if (s == ";") {
    if (op != 0) {
      if (!tmplts.isEmpty()) {
	// have ONE template spec
	insert_template(tmplts.first(), decl, TRUE);
	op->formals = new FormalParameterList(tmplts.first());
	
#ifdef ROUNDTRIP
	if (may_roundtrip) {
	  if (neq(op->stereotype(), "template")) {
	    op->set_Stereotype("template");
	    cl->set_updated();
	  }
	}
	else
#endif
	  op->set_Stereotype("template");
      }
#ifdef ROUNDTRIP
      else if (may_roundtrip && !neq(op->stereotype(), "template")) {
	op->set_Stereotype("");
	cl->set_updated();
      }
      
      if (may_roundtrip) {
	QCString dcl = decl;
	
	if (! friend_template.isEmpty())
	  dcl.insert(dcl.find("${(}"), friend_template);

	if (neq(op->cppDecl(), dcl)) {
	  cl->set_updated();
	  op->set_CppDecl(dcl);
	}
      }
      else
#endif
      if (friend_template.isEmpty())
	op->set_CppDecl(decl);
      else {
	QCString dcl = decl;
	
	dcl.insert(dcl.find("${(}"), friend_template);
	op->set_CppDecl(dcl);
      }
      
      QCString def;
      
      if (! defined_in_decl) {          
	def = CppSettings::operationDef();
	
	int start = decl.find("${(}") + 4;	// decl contains '${(}' and  '${)}'
	int index = def.find("${(}");
	
	if ((index == -1) ||
	    (def.find("${body}", index) == -1) ||
	    (def.find("${type}") == -1) ||
	    (def.find("${class}::${name}") == -1) ||
	    (def.find("${const}", index) == -1) ||
	    (def.find("${volatile}", index) == -1)) {
	  // use a definition where ${body] is not indented
	  def = (decl.find("${throw}") != -1)
	    ? "${comment}${inline}${type} ${class}::${name}${(}${)}${const}${volatile}${throw}${staticnl}{\n${body}}\n"
	    : "${comment}${inline}${type} ${class}::${name}${(}${)}${const}${volatile}${staticnl}{\n${body}}\n";
	  index = def.find("${(}");
	}
	else {
	  if ((decl.find("${throw}") != -1) && (def.find("${throw}") == -1)) {
	    int index2 = def.find("${const}");
	    int index3 = def.find("${volatile}");
	    
	    def.insert((index2 > index3) ? index2 + 8 : index3 + 11, "${throw}");
	  }
	}
	
	if (!pfunc) {
	  def.insert(index + 4, decl.mid(start, decl.find("${)}", start) - start));
	  
	  index = def.find("${type}");
	  
	  if (!modifier.isEmpty())
	    def.insert(index + 7, QCString(" ") + modifier);
	  def.replace(index, 7, typeform);
	}
	else {
	  int index2 = def.find("${)}", index);
	  
	  index = def.find("${type}");
	  
	  type.insert(type.find("${name}"), "${class}::");
	  def.replace(index, index2 + 4 - index,
		      type +
		      decl.mid(start - 4,
			       decl.find("${const}", start + 4) - start + 4));
	}
	
	if (typenamep) {
	  def.insert(index, "typename ");
	  index += 9;
	}
	
	if (constp) {
	  def.insert(index, "const ");
	  index += 6;
	}
	
	if (volatilep)
	  def.insert(index, "volatile ");
	
	if ((name == cl_name) || (name == (QCString("~") + cl_name))) {
	  // constructor or destructor : remove useless ${}
	  index = def.find("${type}");
	  def.remove(index, (((const char *) def)[index + 7] == ' ') ? 8 : 7);
	  def.remove(def.find("${const}"), 8);
	  def.remove(def.find("${volatile}"), 11);
	  def.remove(def.find("${staticnl}"), 11);
	}
	else if (type.isEmpty() && (name.left(9) == "operator ")) {
	  index = def.find("${type}");
	  def.remove(index, (((const char *) def)[index + 7] == ' ') ? 8 : 7);
	}
	
	// remove params default value
	int index2 = def.find("${(}");
	int index3;
	int index4 = def.find("${)}", index2 + 4);
	
	while (((index = def.find("${v", index2)) != -1) &&
	       (index < index4) &&
	       ((index3 = def.find('}', index + 3)) != -1)) {
	  def.remove(index, index3 - index + 1);
	  index2 = index;
	}
      }
#ifdef ROUNDTRIP
      if (may_roundtrip)
	// if the definition wil not be read the definition will
	// be set if needed by mark_useless
	DefNotYetSet.insert(op, new QCString(def));
      else
#endif
	op->set_CppDef(def);
    }
    
    Lex::clear_comments();	// params & body comments
    Lex::finish_line();
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlOperation::read_param(ClassContainer * container, unsigned rank,
			      UmlParameter & param, QCString & decl,
			      const QValueList<FormalParameterList> & tmplt,
			      BooL & on_error, bool add_defaultvalue)
{
#ifdef DEBUG_BOUML
  cout << "UmlOperation::manage_param " << rank << "\n";
#endif
 
  on_error = TRUE;
  Lex::mark();
  
  QCString s = Lex::read_word();

#ifdef DEBUG_BOUML
  cout << "commence par " << s << '\n';
#endif
  
  if (s == "...") {
    decl.insert(decl.find("${)}"), 	// cannot be -1
		(rank == 0) ? "..." : ", ...");
    if (Lex::read_word_bis(TRUE, FALSE) != ')')
      Lex::error_near("...");
    else
      on_error = FALSE;
    return FALSE;
  }
  
  param.name = param.default_value = 0;
  param.type.type = 0;
  param.type.explicit_type = 0;
  
  QCString pn = QCString("${p") + QCString().setNum(rank) + '}';
  bool constp = FALSE;
  bool volatilep = FALSE;
  bool typenamep = FALSE;
  QCString modifier;
  QCString array;
  QCString type;
  QCString pretype;	// struct/union/class/enum
  bool pfct = FALSE;
  
  for (;;) {
    if (s.isEmpty()) {
      Lex::premature_eof();
      return FALSE;
    }
    else if (s == "const") {
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else {
	constp = TRUE;
	Lex::mark();
      }
    }
    else if (s == "volatile") {
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else {
	volatilep = TRUE;
	Lex::mark();
      }
    }
    else if (s == "typename") {
      // note : may have "const typename ...", but not "typename const ..."
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else {
	typenamep = TRUE;
	Lex::mark();
      }
    }
    else if ((s == "unsigned") || (s == "signed") ||
	     (s == "void") || (s == "bool"))
      type = s;
    else if ((s == "char") || (s == "short") || (s == "int") || 
	     (s == "long") || (s == "float") || (s == "double")) {
      type = (type.isEmpty()) ? s : type + ' ' + s;
    }
    else if (Lex::star(s) || (s == "&")) {
      if (!modifier.isEmpty())
	modifier += QCString(" ") + s;
      else if (!type.isEmpty())
	modifier = s;
      else {
	Lex::error_near(s);
#ifdef DEBUG_BOUML
	cout << "ERROR modifier " << s << " et type empty\n";
#endif
	return FALSE;
      }
    }
    else if ((s == ")") || (s == ",") || (s == "...")) {
      if (param.name.isEmpty() && type.isEmpty()) {
	on_error = FALSE;
	return FALSE;
      }
      
      if (s == ")")
	Lex::unread_word();	// ')'
      
      param.dir = (constp || modifier.isEmpty())
	? InputDirection : InputOutputDirection;
      
      QCString s2;
      
      if (rank != 0)
	s2 = ", ";
      if (constp)
	s2 += "const ";
      if (volatilep)
	s2 += "volatile ";
      if (typenamep)
	s2 += "typename ";
      
      QCString n_close = QCString().setNum(rank) + "}";
      
      // note : doesn't add the pretype (class ...) else
      // this will be a problem if this one is not used both
      // in the declaration and definition, furthermore Bouml
      // add needed declaration/includes
      
      if (!pfct) {
	QCString typeform = QCString("${t") + n_close;
	
	container->compute_type(type, param.type, typeform, FALSE, tmplt);
	
	s2 += typeform;
	s2 += ' ';
	if (modifier) 
	  s2 += modifier + ' ';
	s2 += pn;
	s2 += array;
      }
      else {
	param.type.explicit_type =
	  param.type.explicit_type.simplifyWhiteSpace();
	s2 += param.type.explicit_type;
	param.type.explicit_type.remove(param.type.explicit_type.find(pn), 
					pn.length());
      }
	
      if (add_defaultvalue)
	s2 += QCString("${v") + n_close;
      
      if (s == "...")
	s2 += " ...";
      
      decl.insert(decl.find("${)}"), 	// cannot be -1
		  s2);
      on_error = FALSE;
      return TRUE;
    }
    else if (s == "=") {
      // initialized param
      Lex::mark();
      skip_expr("),", TRUE);
      Lex::unread_word();	// ')' or ','
      param.default_value = Lex::region();
      if (*((const char *) param.default_value) == ' ')
	param.default_value = param.default_value.mid(1);
    }
    else if ((s == "struct") ||
	     (s == "union") ||
	     (s == "class") ||
	     (s == "enum")) {
      // suppose struct name ...
      if (! pretype.isEmpty()) {
	Lex::error_near(s);
#ifdef DEBUG_BOUML
	cout << "ERROR '" << s << "' alors qu a deja le pre-type '" << pretype << "'\n";
#endif
	return FALSE;
      }
      else
	pretype = s;
    }
    else if (Lex::identifierp(s, FALSE)) {
      if ((s == "register") || (s == "Q_TYPENAME"))
	;	// lost !
      else if (type.isEmpty()) {
	type = Lex::complete_template_type(s);
	
#ifdef DEBUG_BOUML
	cout << "type = '" << type << "'\n";
#endif
      }
      else if (param.name.isEmpty()) {
	param.name = s;
#ifdef DEBUG_BOUML
	cout << "name = '" << param.name << "'\n";
#endif
      }
      else {
	Lex::error_near(s);
#ifdef DEBUG_BOUML
	cout << "ERROR '" << s << "' alors qu a deja le type '" << type << "' et le nom '" << param.name << "'\n";
#endif
	return FALSE;
      }
    }
    else if (*((const char *) s) == '[')
      array = s;
    else {
      BooL func;
      
      if ((s == "(") &&
	  pfunc(func, param.name, param.type.explicit_type, pn) &&
	  !func)
	pfct = TRUE;
      else {
	Lex::error_near(s);
#ifdef DEBUG_BOUML
	cout << "ERROR : '" << s << "'\n";
#endif
	return FALSE;
      }
    }
    
    s = Lex::read_word();
  }
}

bool UmlOperation::read_throw_elt(ClassContainer * container,
				  UmlTypeSpec & typespec,
				  const QValueList<FormalParameterList> & tmplts)
{
  char c;
  
  while ((c = Lex::read_word_bis(TRUE, FALSE)) != ')') {
    if (c == 0) {
      if (! Package::scanning())
	Lex::premature_eof();
      return FALSE;
    }
      
    if (c == ',') {
      if (! Package::scanning())
	Lex::error_near(",");
    }
    else {
      Lex::unread_word();
      
      QCString s = Lex::normalize(Lex::read_list_elt());
      
      QCString dummy;
      
      container->compute_type(s, typespec, dummy, FALSE, tmplts);
      if (typespec.type == 0)
	typespec.explicit_type = s;
      
      if (Lex::read_word_bis(TRUE, FALSE) != ',')
	Lex::unread_word();
      
      return TRUE;
    }
  }
  
  return FALSE;
}

void UmlOperation::friend_operations(QList<UmlOperation> & candidates,
				     const QValueList<FormalParameterList> & tmplts,
				     const QCString & name)
{
  QList<UmlOperation> * fr = friends[name];
  
  if (fr != 0) {
    // may be a friend operation
    FormalParameterList t;
    const FormalParameterList & tmpl = (tmplts.isEmpty()) ? t : tmplts.last();
    QListIterator<UmlOperation> it(*fr);
      
    while (it.current() != 0) {
      const FormalParameterList & formals =
	((UmlClass *) it.current()->parent())->formals();
      
      if (tmpl.count() <= formals.count()) {
	unsigned rank = tmpl.count();
	
	while (rank--) {
	  if (//??(formals[rank].Name() != tmplt[rank].Name()) ||
	      (formals[rank].type() != tmpl[rank].type())) {
	    break;
	  }
	}
	  
	if (rank == ~0u)
	  // candidate
	  candidates.append(it.current());
      }
      
      ++it;
    }
  }
}

static bool compare_templates(const FormalParameterList & t1,
			      const FormalParameterList & t2,
			      const QCString & w, const QCString & s)
{
  if (t1.count() != t2.count()) {
    Lex::warn("wrong template specification " + w + " <font color =\"red\">" +
	      Lex::quote(s) + "</font>");
#ifdef DEBUG_BOUML
    cout << "ERROR wrong template specification " << w << s << '\n';
#endif
    return FALSE;
  }
  
  unsigned rank = t1.count();
  
  while (rank--) {
    if (t1[rank].name() != t2[rank].name()) {
      Lex::warn("please, help me, use the template specification of <font color =\"red\">"
		+ Lex::quote(s)
		+ "</font>'s declaration, template are already enough complicated !");
#ifdef DEBUG_BOUML
      cout << "ERROR change template names !";
#endif
      return TRUE;	// just a warning
    }
  }
  
  return TRUE;
}

bool UmlOperation::operations(QList<UmlOperation> & candidates, UmlClass * cl,
			      const QValueList<FormalParameterList> & tmplts,
			      const FormalParameterList *& oper_tmplt,
			      const QCString & name)
{
  // name is the full name spec or just a simple if 'using cl' was made
  // normalized is the normalized cl spec
  
  // remove templates linked with cl and may be cl's mother classes from
  // tmplts to know if the operation is a template one
  unsigned bypassed_tmplts = 0;
  UmlItem * cl1 = cl;
  
  do {
    if (!((UmlClass *) cl1)->formals().isEmpty()) {
      // template not explicit
      if (bypassed_tmplts == tmplts.count()) {
	Lex::warn("not enouth template specification for <font color =\"red\">"
		  + Lex::quote(name) + "</font>");
	return TRUE;
      }
      else
	bypassed_tmplts += 1;
    }
  } while ((cl1 = cl1->parent())->kind() == aClass);
    
  // first bypassed_tmplts templace specs must be forgotten
  
  unsigned n = tmplts.count();
  
  if ((n > 1) && (bypassed_tmplts < (n - 1))) {
    Lex::warn("too many template specification for <font color =\"red\">"
	      + Lex::quote(name) + "</font>");
    return TRUE;
  }
    
  // check template spec until the operation's template
  
  unsigned rank;
  
  if (bypassed_tmplts != 0) {
    rank = bypassed_tmplts;
    cl1 = cl;
    
    do {
      if (!((UmlClass *) cl1)->formals().isEmpty()) {
	rank -= 1;
	
	if (!tmplts[rank].isEmpty() && // not a template specialisation
	    !compare_templates(((UmlClass *) cl1)->formals(),
			       tmplts[rank], "for",
			       ((UmlClass *) cl1)->name()))
	  return TRUE;
      }
    } while ((cl1 = cl1->parent())->kind() == aClass);
  }
  
  // search the operation
  
  oper_tmplt = (bypassed_tmplts == n) ? 0 : &tmplts[bypassed_tmplts];
    
  int index;
  QCString opname = ((index = name.findRev("::")) > 0)
    ? name.mid(index + 2)
    : name;
  QVector<UmlItem> children = cl->children();
  
  rank = children.size();
  
  while (rank--) {
    UmlItem * it = children[rank];
    
    if ((it->kind() == aRelation) ? (((UmlRelation *) it)->roleName() == opname)
				  : (it->name() == opname)) {
      switch (it->kind()) {
      case anOperation:
	// a candidate, it is useless to look at the returned value
	// bypass template operations (their decl is empty)
	if (((((UmlOperation *) it)->formals != 0)
	     ? (oper_tmplt != 0) : (oper_tmplt == 0)) &&
	    !((UmlOperation *) it)->cppDecl().isEmpty())
	  candidates.append((UmlOperation *) it);
      default:	// to avoid compiler warning
	break;
      case anAttribute:
      case aRelation:
	if (oper_tmplt != 0)
	  Lex::warn("too many template specification for <font color =\"red\">"
		    + Lex::quote(name) + "</font>");
	else if (((UmlClassMember *) it)->isClassMember()) {
	  // the form is not an operation definition,
	  // it is a class variable initialisation
	  Lex::mark();
	  
	  char c;
	  
	  while ((c = Lex::read_word_bis(TRUE, FALSE)) != ';') {
	    if (c == 0) {
	      Lex::premature_eof();
	      return TRUE;
	    }
	  }
	  
	  QCString e = QCString("(") + Lex::region();
	  
	  e.truncate(e.length() - 1);
	  
#ifdef ROUNDTRIP
	  if (cl->is_roundtrip_expected()) {
	    if (it->kind() == anAttribute) {
	      if (nequal(((UmlAttribute *) it)->defaultValue(), e)) {
		((UmlAttribute *) it)->set_DefaultValue(e);
		cl->get_class()->set_updated();;
	      }
	    }
	    else {
	      if (nequal(((UmlRelation *) it)->defaultValue(), e)) {
		((UmlRelation *) it)->set_DefaultValue(e);
		cl->get_class()->set_updated();;
	      }
	    }
	  }
	  else
#endif
	  if (it->kind() == anAttribute)
	    ((UmlAttribute *) it)->set_DefaultValue(e);
	  else
	    ((UmlRelation *) it)->set_DefaultValue(e);
	}
	else
	  Lex::warn("<font color =\"red\"> " + Lex::quote(name) +
		    "</font> is not a static variable");
	return FALSE;
      }
    }
  }
  
  if (candidates.isEmpty() && (n != 0))
    Lex::warn("doesn't match any template declaration");

  return TRUE;
}
  
void UmlOperation::reverse_definition(Package * pack, QCString name,
				      QCString type, 
				      QValueList<FormalParameterList> & tmplts,
				      bool inlinep, const QCString & comment,
				      const QCString & description)
{
  // parameter list '(' already read
  
  int index;
  bool pfct = FALSE;
  QCString normalized;
  UmlTypeSpec tcl;
    
  if (name.isEmpty()) {
    if (type.isEmpty()) {
      if (!Package::scanning())
	Lex::error_near("(");
      UmlOperation::skip_body();
      return;
    }
    
    if ((index = type.find("::operator ")) != -1) {
      name = type;
      type = 0;
    }
    else {
      QCString s1;
      QCString s2;
    
      if (((index = type.findRev("::")) > 0) &&
	  pack->find_type(normalized = Lex::normalize(type.left(index)), tcl) &&
	  (s1 = tcl.type->name(),
	   s2 = type.mid(index + 2),
	   s1 = ((index = s1.find('<')) != -1) ? s1.left(index) : s1,
	   ((s1 == s2) || (s2 == (QCString("~") + s1))))) {
	name = type;
	type = 0;
      }
      else {
	BooL func;
	
	Lex::mark();	// can be made only here because return_type is not significant
	
	if (!UmlOperation::pfunc(func, name, type, "${type}")) {
	  UmlOperation::skip_body();
	  return;
	}
	else if (!func) {
	  // not an operation : a variable valuing a function/operation pointer
	  if (Lex::read_word_bis(TRUE, FALSE) == '=')
	    pack->reverse_variable(name);
	  return;
	}
	else
	  pfct = TRUE;
      }
    }
  }
  
  QList<UmlOperation> candidates;
  const FormalParameterList * oper_tmplt;
  
  if ((tcl.type != 0) ||	// set in case of a contructor/destructor
      (((index = name.findRev("::")) > 0) &&
       pack->find_type(normalized = Lex::normalize(name.left(index)), tcl))) {
    if (! operations(candidates, tcl.type, tmplts, oper_tmplt, name))
      // a variable, already managed inside operations
      return;
  }
  else {
    friend_operations(candidates, tmplts, name);
    oper_tmplt = 0;
  }
  
#ifdef DEBUG_BOUML
  cout << candidates.count() << " compatible operations for " << name << '\n';
#endif

  if (candidates.isEmpty()) {
    Lex::warn("<font color =\"red\"> " + Lex::quote(name) + "</font> is lost");
    UmlOperation::skip_body();
    return;
  }

  QListIterator<UmlOperation> it(candidates);
  LexContext context = Lex::get_context();
  BooL on_error = FALSE;
  unsigned nargs = ~0u;
  UmlClass * cl = 0;
  
  do {
    if ((nargs == ~0u) || (it.current()->params().count() == nargs)) {
      if (cl != it.current()->parent()) {
	cl = (UmlClass *) it.current()->parent();
	cl->set_under_construction(TRUE, TRUE);
      }
      
      if (it.current()->reverse_if_def(pack, tmplts, oper_tmplt, inlinep, 
				       pfct, comment, description,
				       on_error, nargs, name)) {
	cl->set_under_construction(FALSE, TRUE);
	return;
      }
      Lex::set_context(context);
    }
    ++it;
  } while (!on_error && (it.current() != 0));

  cl->set_under_construction(FALSE, TRUE);
  
  // no compatible operation
  Lex::warn("<font color =\"red\"> " + Lex::quote(name) + "</font> is lost");
  
  skip_body(0);
}

bool UmlOperation::reverse_if_def(Package * pack,
				  QValueList<FormalParameterList> & tmplts,
				  const FormalParameterList * oper_tmplt,
				  bool inlinep, bool pfct, const QCString & comment,
				  const QCString & description, BooL & on_error,
				  unsigned & nargs, QCString oper_name) {
  // parameters
  
  UmlParameter param;
  QValueList<UmlParameter> params = this->params();
  QList<QCString> param_names;
  QCString decl = "${(}${)}";
  unsigned rank = 0;
  
  param_names.setAutoDelete(TRUE);
  
  while (read_param(pack /*yes !*/, rank, param, decl, tmplts, on_error, FALSE)) {
    if ((params.count() <= rank) ||
	(params[rank].dir != param.dir) ||
	(params[rank].type.toString() != param.type.toString()))
      // uncompatible
      return FALSE;
    
    params[rank].name = param.name; // may be !=
    rank += 1;
  }
  
  if (on_error)
    return FALSE;
  
  nargs = rank;
  
  // not do not check that 'this' does not have too many
  // parameters, this test is included in the parameters decl check
  
  QCString s;
  
  if (pfct) {
    if (((s = Lex::read_word()) != ")") ||
	((s = Lex::read_word()) != "(")) {
      Lex::error_near(s);
      on_error = TRUE;
      return FALSE;
    }
    skip_expr(")");
  }

  // const
  
  Lex::mark();
  
  s = Lex::read_word();
  
  // const may be before volatile
  bool is_const;
  
  if (s == "const") {
    is_const = TRUE;
    Lex::mark();
    s = Lex::read_word();
  }
  else
    is_const = FALSE;

  if (s == "volatile") {
    if (!isVolatile())
      // uncompatible
      return FALSE;
    
    Lex::mark();
    s = Lex::read_word();
  }
  else if (isVolatile())
    // uncompatible
    return FALSE;

  // const may be after volatile
  if (is_const || (s == "const")) {
    if (!isCppConst())
      // uncompatible
      return FALSE;
    
    if (s == "const"){
      Lex::mark();
      s = Lex::read_word();
    }
  }
  else if (isCppConst())
    // uncompatible
    return FALSE;

  // throw
  
  const QValueList<UmlTypeSpec> exc = exceptions();
  
  if (s == "throw") {
    if (Lex::read_word_bis(TRUE, FALSE) != '(') {
      Lex::error_near("throw");
      on_error = TRUE;
      return FALSE;
    }
    
    UmlTypeSpec tcl;
    
    rank = 0;
      
    while (read_throw_elt(pack, tcl, tmplts)) {
      if ((exc.count() <= rank) || (exc[rank].toString() != tcl.toString()))
	// uncompatible
	return FALSE;
      
      rank += 1;
    }
    
    if (exc.count() != rank)
      // uncompatible
      return FALSE;
    
    Lex::mark();
    s = Lex::read_word();
  }
  else if (!exc.isEmpty())
    // uncompatible
    return FALSE;
  
  // check the parameters decl to take into account the modifiers

#ifdef ROUNDTRIP
  QCString * fromdecl = DefNotYetSet[this];
  QCString def = (fromdecl == 0) ? cppDef() : *fromdecl;
#else
  QCString def = cppDef();
#endif
  int start = def.find("${(}");
  int end = def.find("${)}");
  
  if (def.mid(start, end - start + 4).simplifyWhiteSpace() ==
      decl.simplifyWhiteSpace()) {
    // operation find !
    if (formals == 0) {
      if (oper_tmplt != 0) {
	Lex::warn("too many template specification for <font color =\"red\">"
		  + Lex::quote(name()) + "</font>");
	on_error = TRUE;
	return FALSE;
      }
    }
    else if (oper_tmplt == 0) {
      Lex::warn("not enouth template specification for <font color =\"red\">"
		+ Lex::quote(name()) + "</font>");
      on_error = TRUE;
      return FALSE;
    }
    else if (! compare_templates(*formals, *oper_tmplt, "in", name())) {
      on_error = TRUE;
      return FALSE;
    }
    
    UmlOperation * op;
    bool def_modified = FALSE;
#ifdef ROUNDTRIP
    Class * cl = ((UmlClass *) parent())->get_class();
    bool roundtrip = FALSE;
#endif
    
    if (tmplts.isEmpty() || !tmplts.last().isEmpty()) {
      update_param_names(params);
      op = this;
#ifdef ROUNDTRIP
      roundtrip = op->is_roundtrip_expected();
#endif
    }
    else {
#ifdef ROUNDTRIP
      while (params.count() != nargs) {
#if QT_VERSION == 230
	QValueList<UmlParameter>::Iterator it = params.end();
	
	--it;
	params.remove(it);
#else
	params.pop_back();
#endif
      }
      
      if (cl->reversed() && 
	  ((op = already_exist(cl, this->name(), params, TRUE)) != 0)) {
	// update already existing operation
	roundtrip = TRUE;
	op->set_usefull();
	if (!op->returnType().equal(returnType())) {
	  cl->set_updated();
	  op->set_ReturnType(returnType());
	}
	
	op->update_params(cl, params);
	op->update_exceptions(cl, exceptions());
      }
      else {
	if (cl->reversed())
	  cl->set_updated();
#endif
	// template operation
	// create a new operation like 'this' to support it
	op = UmlBaseOperation::create((UmlClass *) parent(), this->name());
	
	op->set_CppDecl("");	// no decl
	op->set_ReturnType(returnType());
	
	// parameters
	QValueList<UmlParameter>::ConstIterator itp;
	unsigned rk;
	
	for (itp = params.begin(), rk = 0; itp != params.end(); ++itp, rk += 1)
	  op->addParameter(rk, *itp);
	
	// exceptions
	const QValueList<UmlTypeSpec> exc = exceptions();
	QValueList<UmlTypeSpec>::ConstIterator ite;
	
	for (ite = exc.begin(), rk = 0; ite != exc.end(); ite++, rk += 1)
	  op->addException(rk, *ite);
#ifdef ROUNDTRIP
      }
#endif
      
      // oper_name is class<...>::oper
      QCString s = oper_name.left(oper_name.findRev("::"));
      
      int index = s.findRev("::");

      s = // <...>
	s.mid(s.find('<', (index == -1) ? 0 : index + 2));

      if (! def0.isEmpty())
	def = def0;
      
      if ((index = def.find("${class}")) != -1) {
	def.insert(index + 8, s);
	def_modified = TRUE;
      }
    }
    
    // manage body
    
    if (s == ":") {
      // constructor
      char c;
      
      while ((c = Lex::read_word_bis(TRUE, TRUE)) != '{') {
	if (c == 0) {
	  on_error = TRUE;
	  return FALSE;
	}
      }
      
      s = Lex::region();	// including ':'
      if (op == this)
	def0 = def;
      def.insert(end + 4, s.left(s.length() - 1));	// without {
      def_modified = TRUE;
      
      s = "{";
    }
    
    if (s == "{") {
      Lex::mark();
      skip_body(1);
      s = Lex::region();
      s.truncate(s.length() - 1);	// remove }
      
      // remove fist \n
      if (*((const char *) s) == '\n')
	s.remove(0, 1);
      
      // remove last spaces and tabs
      int ln = s.length();
      
      while (ln && ((s[ln - 1] == ' ') || (s[ln - 1] == '\t')))
	ln -= 1;
      s.truncate(ln);
      if (!s.isEmpty() && (s[ln - 1] != '\n'))
	s += "\n";
      
#ifdef ROUNDTRIP
      if (roundtrip) {
	clean_body(s);
	
	// remove \r in case of preserve body
	QCString current_body = op->cppBody();
	int index = 0;
	
	while ((index = current_body.find('\r', index)) != -1)
	  current_body.remove(index, 1);
	
	if (nequal(current_body, s)) {
	  cl->set_updated();
	  op->set_CppBody(s);
	  op->set_CppContextualBodyIndent(FALSE);
	}
	
	if (inlinep != op->isCppInline()) {
	  cl->set_updated();
	  op->set_isCppInline(inlinep);
	}
      }
      else {
#endif
	op->set_CppBody(s);
	op->set_CppContextualBodyIndent(FALSE);
	
	if (inlinep)
	  op->set_isCppInline(TRUE);
#ifdef ROUNDTRIP
      }
#endif
      
      if (formals != 0) {
	if (def0.isEmpty() && (op == this))
	  def0 = def;
	insert_template(*formals, def, FALSE);
	def_modified = TRUE;
      }
            
      s = comment;
      
      if (!s.isEmpty() &&
	  nequal(op->description(), description) &&
	  nequal(op->description(), Lex::simplify_comment(s))) {
	int index;
	
	if ((index = def.find("${description}")) != -1) {
	  if (def0.isEmpty() && (op == this))
	    def0 = def;
	  def.remove(index, 14);
	  def.insert(0, description);
	  def_modified = TRUE;
	}
	else {
	  start = 0;
	  do {
	    s.insert(start, "//");
	    start = s.find('\n', start + 2) + 1;
	  } while (start != 0);
	  if (neq(s, op->description())) {
	    s.append("\n\n");
	    if (def0.isEmpty() && (op == this))
	      def0 = def;
	    if ((index = def.find("${comment}")) != -1)
	      def.remove(index, 10);
	    def.insert(0, s);
	    def_modified = TRUE;
	  }
	}
      }
      
#ifdef ROUNDTRIP
      if (op == this)
	DefNotYetSet.remove(this);
      
      if (roundtrip) {
	if (neq(def, op->cppDef())) {
	  cl->set_updated();
	  op->set_CppDef(def);
	}
      }
      else
#endif
	if (def_modified)
	  op->set_CppDef(def);
      
      Lex::clear_comments();
      return TRUE;
    }
    else
      on_error = TRUE;
  }

  return FALSE;
}

void UmlOperation::skip_body(int level) {
  char e;
    
  while ((e = Lex::read_word_bis(TRUE, TRUE)) != 0) {	// not end of file
    if (e == '}') {
      if (--level == 0)
	// end of body
	break;
    }
    else if (e == ';') {
      if (level == 0)
	// no body, probably a declaration
	break;
    }
    else if (e == '{')
      level += 1;
  }
  
  Lex::clear_comments();
}

void UmlOperation::skip_expr(QCString end, bool allow_templ) {
  char c;
  int level = 0;
    
  for (;;) {
    c = Lex::read_word_bis(TRUE, TRUE);
    
    if (c == 0)
      break;
    
    if ((end.find(c) != -1) && (level <= 0))
      break;
    
    switch (c) {
    case '<':
      if (! allow_templ)
	break;
      // no break
    case '(':
    case '{':
      level += 1;
      break;
    case '>':
      if (! allow_templ)
	break;
      // no break
    case ')':
    case '}':
    case ']':
      level -= 1;
    }
  }
  
  Lex::clear_comments();
}

#ifdef REVERSE
bool UmlOperation::need_source() {
  return !isCppInline();
}
#endif

void UmlOperation::update_param_names(QValueList<UmlParameter> & params) {
  QValueList<UmlParameter>::Iterator itdef;
  const QValueList<UmlParameter> decl_params = this->params();
  QValueList<UmlParameter>::ConstIterator itdecl;
  unsigned rank;
  
  for (rank = 0, itdef = params.begin(), itdecl = decl_params.begin();
       (itdef != params.end()) && (itdecl != decl_params.end());
       ++itdef, ++itdecl, rank += 1) {
    UmlParameter & pdef = *itdef;
    
    if (!pdef.name.isEmpty() && (pdef.name != (*itdecl).name)) {
      UmlParameter p = *itdecl;
      
      p.name = pdef.name;
      replaceParameter(rank, p);
    }
  }
}

#ifdef ROUNDTRIP
void UmlOperation::update_params(Class * cl,
				 QValueList<UmlParameter> & params) {
  QValueList<UmlParameter>::Iterator itp1;
  const QValueList<UmlParameter> old_params = this->params();
  QValueList<UmlParameter>::ConstIterator itp2;
  unsigned rank;
  
  for (rank = 0, itp1 = params.begin(), itp2 = old_params.begin();
       (itp1 != params.end()) && (itp2 != old_params.end());
       ++itp1, ++itp2, rank += 1) {
    UmlParameter & p1 = *itp1;
    const UmlParameter & p2 = *itp2;
    
    if (((p1.name.isEmpty()) ? !p2.name.isEmpty() : (p1.name != p2.name)) ||
	nequal(p1.default_value, p2.default_value) ||
	!p1.type.equal(p2.type)) {
      if (p1.name.isEmpty())
	p1.name = p2.name;
      if (p1.dir != InputDirection)
	p1.dir = p2.dir;
      replaceParameter(rank, p1);
      cl->set_updated();
    }
    else if ((p1.dir == InputDirection) && (p2.dir != InputDirection)) {
      replaceParameter(rank, p1);
      cl->set_updated();
    }
  }
       
  if (itp1 != params.end()) {
    // have missing params
    cl->set_updated();
    do {
      addParameter(rank, *itp1);
      itp1++;
      rank += 1;
    } while (itp1 != params.end());
  }
  else if (itp2 != old_params.end()) {
    // have extra params
    cl->set_updated();
    do {
      removeParameter(rank);
      itp2++;
    } while (itp2 != old_params.end());
  }
}

void UmlOperation::update_exceptions(Class * cl,
				     const QValueList<UmlTypeSpec> & exceptions) {
  QValueList<UmlTypeSpec>::ConstIterator ite1;
  const QValueList<UmlTypeSpec> old_exceptions = this->exceptions();
  QValueList<UmlTypeSpec>::ConstIterator ite2;
  unsigned rank;
  
  for (rank = 0, ite1 = exceptions.begin(), ite2 = old_exceptions.begin();
       (ite1 != exceptions.end()) && (ite2 != old_exceptions.end());
       ++ite1, ++ite2, rank += 1) {
    const UmlTypeSpec & e1 = *ite1;
    
    if (!e1.equal(*ite2)) {
      replaceException(rank, e1);
      cl->set_updated();
    }
  }
  
  if (ite1 != exceptions.end()) {
    // have missing exceptions
    cl->set_updated();
    do {
      addException(rank, *ite1);
      ite1++;
      rank += 1;
    } while (ite1 != exceptions.end());
  }
  else if (ite2 != old_exceptions.end()) {
    // have extra exceptions
    cl->set_updated();
    do {
      removeException(rank);
      ite2++;
    } while (ite2 != old_exceptions.end());
  }
}

void UmlOperation::clean_body(QCString & body)
{
  const char * BodyPrefix = "// Bouml preserved body begin ";
  const char * BodyPostfix = "// Bouml preserved body end ";
  const int BodyPrefixLength = 30;

  int index = body.find(BodyPrefix);
  
  if (index != -1) {
    const char * b = 
      strchr(((const char *) body) + index + BodyPrefixLength, '\n');

    if (b == 0) {
      // invalid
      body = "";
      return;
    }
    const char * e = strstr(b, BodyPostfix);
    
    if (e != 0) {
      while ((e != b) && (e[-1] != '\n'))
	e -= 1;
      
      body = body.mid(b - (const char *) body, e - b);  
    }
  }
}

UmlOperation * UmlOperation::already_exist(Class * container,
					   const QCString & name,
					   QValueList<UmlParameter> & params,
					   bool empty_decl)
{
  const QVector<UmlItem> & ch = container->get_uml()->UmlItem::children();
  UmlItem ** v = ch.data();
  UmlItem ** const vsup = v + ch.size();
  QList<UmlOperation> opers;
  
  for (;v != vsup; v += 1)
    if (((*v)->kind() == anOperation) &&
	((*v)->name() == name) &&
	((UmlOperation *) *v)->is_useless() &&
	(((UmlOperation *) *v)->cppDecl().isEmpty() == empty_decl))
      opers.append((UmlOperation *) *v);
  
  switch (opers.count()) {
  case 0:
    return 0;
  case 1:
    // suppose it is this one
    // even don't know if it is placed later in file
    return opers.getFirst();
  default:
    break;
  }
    
  UmlOperation * op;
  QList<UmlOperation> same_names;

  // search for operation having the same params name and number
  for (op = opers.first(); op != 0; op = opers.next()) {
    QValueList<UmlParameter> ps = op->params();
    QValueList<UmlParameter>::ConstIterator it1;
    QValueList<UmlParameter>::ConstIterator it2;
    bool same_type = TRUE;
    bool same_name = TRUE;
    
    for (it1 = params.begin(), it2 = ps.begin();
	 (it1 != params.end()) && (it2 != ps.end());
	 ++it1, ++it2) {
      const UmlParameter & p1 = *it1;
      const UmlParameter & p2 = *it2;
      
      if ((p1.name.isEmpty()) ? !p2.name.isEmpty() : (p1.name != p2.name)) {
	same_name = FALSE;
	break;
      }
      
      if (!p1.type.equal(p2.type))
	same_type = FALSE;
    }
    
    if (same_name && (it1 == params.end()) && (it2 == ps.end())) {
      if (same_type)
	// perhaps several has the same number of param
	// and same param names and type because of [] 
	// not managed here, suppose the right is this one
	return op;
      same_names.append(op);
    }
  }
  
  if (same_names.count() == 1) {
    // only one having the same number of param
    // and same param names (type changed)
    // suppose this one
    return same_names.getFirst();
  }
  
  // suppose not find
  return 0;
}

void UmlOperation::force_defs()
{
  QPtrDictIterator<QCString> it(DefNotYetSet);
  
  for (; it.current(); ++it) {
    UmlOperation * op = (UmlOperation *) it.currentKey();
    
    if (neq(*(it.current()), op->cppDef())) {
      op->set_CppDef(*(it.current()));
      ((UmlClass *) op->parent())->get_class()->set_updated();
    }
  }
  
  DefNotYetSet.setAutoDelete(TRUE);
  DefNotYetSet.clear();
}

#endif
