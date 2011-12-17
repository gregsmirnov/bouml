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

#include "UmlClass.h"
#include "UmlPackage.h"
#include "Package.h"
#include "ClassContainer.h"
#include "Lex.h"
#include "UmlRelation.h"
#include "UmlOperation.h"
#include "CppSettings.h"
#include "UmlArtifact.h"
#include "UmlDeploymentView.h"
#include "Statistic.h"
#include "UmlCom.h"
#ifdef REVERSE
#include "CppCatWindow.h"
# ifdef ROUNDTRIP
#include "UmlExtraClassMember.h"
#include "Class.h"
# endif
#endif

// contains the classes whose definition in currently read
// may be more than one because of the nested classes
QList<UmlClass> UmlClass::UnderConstruction;

// used (by using) classes list
QDict<UmlClass> UmlClass::Usings;

// to lost usings defined under a namespace/class block
QValueList<QDict<UmlClass> > UmlClass::UsingScope;

UmlClass::UmlClass(void * id, const QCString & n) 
    : UmlBaseClass(id, n)
#ifdef ROUNDTRIP
    , created(FALSE), the_class(0)
#endif
{}

bool UmlClass::manage_inherit(ClassContainer * container, 
			      const QValueList<FormalParameterList> & tmplts
#ifdef REVERSE
			      , bool libp
# ifdef ROUNDTRIP
			      , bool roundtrip, QList<UmlItem> & expected_order
			      , bool container_roundtrip, QList<UmlItem> & container_expected_order
# endif
#endif
			      ) {
#ifdef DEBUG_BOUML
  cout << name() << "->manage_inherit()\n";
#endif
  
  QCString s = Lex::read_word(TRUE);
  
  while (s != "{") {
#ifdef DEBUG_BOUML
    cout << "Class::manage_inherit, visibility : " << s << '\n';
#endif
    
    bool is_virtual;
    
    if (s == "virtual") {
      is_virtual = TRUE;
      s = Lex::read_word(TRUE);
    }
    else
      is_virtual = FALSE;
      
    aVisibility v;
    
    if (s == "public") {
      v = PublicVisibility;
      s = Lex::read_word(TRUE);
    }
    else if (s == "protected") {
      v = ProtectedVisibility;
      s = Lex::read_word(TRUE);
    }
    else if (s == "private") {
      v = PrivateVisibility;
      s = Lex::read_word(TRUE);
    }
    else
      v = PrivateVisibility;
        
    if (s.isEmpty()) {
      Lex::premature_eof();
      return FALSE;
    }
    
#ifdef DEBUG_BOUML
    cout << "UmlClass::manage_inherit, mother : " << s << '\n';
#endif
    
    QCString mother_name = s;
    UmlTypeSpec mother;
    QCString typeform;
    UmlRelation * rel = 0;
#ifdef ROUNDTRIP
    bool is_new = TRUE;
#endif
    
    container->compute_type(s, mother, typeform);
    
    s = Lex::read_word();
    
    if (s == "<") {
      Lex::mark();

      // goes up to the corresponding '>'
      QCString after_gt;
      
      Lex::finish_template(after_gt);

      s = Lex::read_word(TRUE);

      if (*s == ':') {
	// inherits 'T<...>::...'
	// don't try to solve, use a typedef named 'Type_<n>' based on T<...>::...
	// creating this typedef if it doesn't yet exist
	
	mother_name += "<" + Lex::region();
	mother.type = 0;  // made below
        s = Lex::read_word();
      }
      else if (mother.type == 0) {
	mother_name += "<" + after_gt;
      }
      else {
	// inherits T<...>
#ifdef ROUNDTRIP
	is_new = !roundtrip || ((rel = search_for_inherit(mother.type)) == 0);
#endif	
	mother_name += "<" + after_gt;
	Lex::come_back();
	
	// must add inheritance before setting actuals
	if (
#ifdef ROUNDTRIP
	    is_new &&
#endif
	    ((rel = UmlBaseRelation::create(aRealization, this, mother.type)) == 0)) {
	  Lex::warn("cannot inherit <font color =\"red\">" +
		    Lex::quote(mother_name) +" </font>");
#ifdef DEBUG_BOUML
	  cout << "cannot create <|---\n";
#endif
	  return FALSE;
	}
	else if (!get_actuals(mother.type, container, tmplts
#ifdef ROUNDTRIP
			      , !is_new
#endif
			      ))
	  return FALSE;
	
#ifdef ROUNDTRIP
	if (! is_new) {
	  if (neq(rel->stereotype(), "bind")) {
	    rel->set_Stereotype("bind");
	    the_class->set_updated();
	  }
	}
	else
#endif
	  rel->set_Stereotype("bind");
	
        s = Lex::read_word();
      }
    }
    
    if (mother.type == 0) {
      mother.type = auxilarily_typedef(mother_name
#ifdef REVERSE
				       , libp
# ifdef ROUNDTRIP
				       , container_roundtrip
				       , container_expected_order
# endif
#endif
				       );
      if (mother.type == 0)
	return FALSE;
    }
    
#ifdef ROUNDTRIP
    if (rel == 0)
      is_new = !roundtrip || ((rel = search_for_inherit(mother.type)) == 0);
#endif
    
    if ((rel == 0) &&
        ((rel = UmlBaseRelation::create(aGeneralisation, this, mother.type)) == 0)) {
      Lex::warn("cannot inherit <font color =\"red\">" +
		Lex::quote(mother_name) +" </font>");
#ifdef DEBUG_BOUML
      cout << "cannot create <|---\n";
#endif
      return FALSE;
    }
    
#ifdef ROUNDTRIP
    expected_order.append(rel);
    
    if (!is_new) {
      rel->set_usefull();
      
      if (neq(rel->cppDecl(), "${type}")) {
	rel->set_CppDecl("${type}");
	the_class->set_updated();
      }
    
      if (rel->visibility() != v) {
	rel->set_Visibility(v);
	the_class->set_updated();
      }
      
      if (is_virtual != rel->cppVirtualInheritance()) {
	rel->set_CppVirtualInheritance(is_virtual);
	the_class->set_updated();
      }
    }
    else {
#elif defined(REVERSE)
      Statistic::one_relation_more();
#endif
      rel->set_CppDecl("${type}");
      rel->set_Visibility(v);
      if (is_virtual)
	rel->set_CppVirtualInheritance(TRUE);
#ifdef ROUNDTRIP
    }
#endif
    
    if (s == ",")
      s = Lex::read_word();
  }
  
  Lex::unread_word();	// '{'
  
  return TRUE;
}

UmlClass * UmlClass::auxilarily_typedef(const QCString & base
#ifdef REVERSE
					, bool libp
# ifdef ROUNDTRIP
					, bool container_roundtrip
					, QList<UmlItem> & container_expected_order
# endif
#endif
					) {
  QCString typedef_decl = CppSettings::typedefDecl();
  const QVector<UmlItem> & children = parent()->children();
  unsigned n = children.count();
  unsigned index;
  
  // a typedef with the right definition already exist ?
  for (index = 0; index != n; index += 1) {
    if (children[index]->kind() == aClass) {
      UmlClass * cl = (UmlClass *) children[index];

      if ((cl->stereotype() == "typedef") &&
	  (cl->cppDecl() == typedef_decl) &&
	  (cl->baseType().explicit_type == base)) {
#ifdef ROUNDTRIP
	cl->set_usefull();
	if (container_roundtrip)
	  container_expected_order.append(cl);
#endif
	return cl;
      }
    }
  }

  // must create typedef with a new name
  for (;;) {
    static unsigned nty;

    QCString s;
      
    s.sprintf("typedef%u", ++nty);

    for (index = 0; index != n; index += 1)
      if (children[index]->name() == s)
	break;

    if (index == n) {
      UmlClass * cl = UmlClass::create(parent(), s);

      if (cl == 0) {
#ifdef REVERSE
	UmlCom::message("");
	CppCatWindow::trace(QCString("<font face=helvetica><b>cannot create class <i>")
			    + s + "</i> under <i>"
			    + parent()->name() + "</b></font><br>");
	throw 0;
#else
	QMessageBox::critical(0, "Fatal Error", 
			      QCString("<font face=helvetica><b>cannot create class <i>")
			      + s + "</i> under <i>"
			      + parent()->name() + "</b></font><br>");
	QApplication::exit(1);
#endif
      }

      UmlTypeSpec typespec;

      typespec.explicit_type = base;
      cl->set_Stereotype("typedef");
      cl->set_BaseType(typespec);
      cl->set_CppDecl(typedef_decl);
#ifdef REVERSE
      if (!libp)
	cl->need_artifact(Namespace::current());
# ifdef ROUNDTRIP
      if (container_roundtrip)
	container_expected_order.append(cl);
# endif
#endif      

      return cl;
    }
  }
}

bool UmlClass::get_actuals(UmlClass * mother, ClassContainer * container, 
			   const QValueList<FormalParameterList> & tmplts
#ifdef ROUNDTRIP
			   , bool roundtrip
#endif
			   ) {
  // < read  
  const QValueList<UmlActualParameter> actuals = this->actuals();
  QValueList<UmlActualParameter>::ConstIterator it;
  unsigned rank;
  
  for (it = actuals.begin(), rank = 0; (*it).superClass() != mother; ++it, rank += 1)
    if (it == actuals.end())
      break;
  
  char c;
      
  while ((c = Lex::read_word_bis(TRUE, FALSE)) != 0) {
    if (c == '>')
      return TRUE;
    
    if (c == ',') {
      // value not given
      ++it;
      rank += 1;
    }
    else {
      if ((it == actuals.end()) || ((*it).superClass() != mother)) {
	Lex::warn("too many actuals for <font color =\"red\">" +
		  Lex::quote(mother->name()) + "</font>");
	return FALSE;
      }
      
      Lex::unread_word();
      
      QCString s = Lex::read_list_elt();
      
      UmlTypeSpec typespec;
      QCString typeform = "${type}";
	
      container->compute_type(s, typespec, typeform, FALSE, tmplts);
      if (typespec.explicit_type == "<complex type>")
	typespec.explicit_type = typeform;
#ifdef ROUNDTRIP
      if (roundtrip) {
	if (!(*it).value().equal(typespec)) {
	  the_class->set_updated();
	  replaceActual(rank, typespec);
	}
      }
      else
#endif
	replaceActual(rank, typespec);
      ++it;
      rank += 1;
      
      c = Lex::read_word_bis(TRUE, FALSE);
      if (c == 0)
	break;
      else if (c != ',')
	Lex::unread_word();
    }
  }
  
  Lex::premature_eof();
  return FALSE;
}

void UmlClass::set_under_construction(bool y, bool rec) {
  if (y) {
    if (rec) {
      UmlItem * it = this;
      
      do {
	UnderConstruction.append((UmlClass *) it);
      } while ((it = it->parent())->kind() == aClass);
    }
    else
      UnderConstruction.append(this);
  }    
  else if (rec)
    UnderConstruction.clear();
  else
    UnderConstruction.removeRef(this);
}

bool UmlClass::inside_its_definition() {
  return UnderConstruction.findRef(this) != -1;
}

bool UmlClass::is_itself(QCString t) {
  // class is a template class and t is x<...> where x is the class,
  // t is normalized
  // return true if t is the class with its formals
  int index = t.find('<');
  
  t = t.mid(index + 1, t.length() - index - 2);
  
  QValueList<UmlFormalParameter> l = formals();
  QValueList<UmlFormalParameter>::ConstIterator it = l.begin();
  QCString t2 = (*it).name();
  
  while ((++it) != l.end())
    t2 += ',' + (*it).name();

  return (t == t2);
}

void UmlClass::restore_using_scope()
{ 
  Usings = UsingScope.first();
  UsingScope.remove(UsingScope.begin());
}

#ifdef REVERSE

void UmlClass::need_artifact(const QCString & nmsp) {
  if (parent()->kind() == aClassView) {
    UmlArtifact * cp;
    
    if ((cp = associatedArtifact()) == 0) {
      // search artifact
#ifdef ROUNDTRIP
      if ((cp = Package::get_artifact()) != 0) {
	cp->addAssociatedClass(this);
	return;
      }
#endif
      
      QCString name = Package::get_fname();
      int index;
	
      if (name.isEmpty()) {
	// defined in a source rather than a header
	// will create it own artifact
	name = this->name();
	index = name.find('<');
      
	if (index != -1)
	  name = name.left(index);
      }
      
      UmlDeploymentView * cpv =
	((UmlPackage *) parent()->parent())->get_deploymentview(nmsp);
      const QVector<UmlItem> & children = cpv->children();
      int n = (int) children.count();
      
      for (index = 0; index != n; index += 1) {
	if ((children[index]->name() == name) &&
	    (children[index]->kind() == anArtifact)) {
	  cp = (UmlArtifact *) children[index];
	  break;
	}
      }
      
      if (cp == 0) {
	// does not exist, create associated artifact
	if ((cp = UmlBaseArtifact::create(cpv, name))
	    == 0) {
#ifdef REVERSE
	  UmlCom::trace(QCString("<font face=helvetica><b>cannot create artifact <i>")
			+ Lex::quote(name) + "</i> under <i>"
			+ Lex::quote(cpv->name()) +
			"</b></font><br>");
	  UmlCom::message("");
	  throw 0;
#else
	  QMessageBox::critical(0, "Fatal Error", 
				QCString("<font face=helvetica><b>cannot create artifact <i>")
				+ Lex::quote(name) + "</i> under <i>"
				+ Lex::quote(cpv->Name()) + "</b></font><br>");
	  QApplication::exit(1);
#endif	  
	}
	
	cp->set_Stereotype("source");
	cp->set_CppHeader(CppSettings::headerContent());
      }
      
      cp->addAssociatedClass(this);
    }
    
    // source needed ?
    
    if (need_source() && cp->cppSource().isEmpty())
      cp->set_CppSource(CppSettings::sourceContent());
  }
}

bool UmlClass::need_source() {
  const QCString & stereotype = this->stereotype();
    
  if ((stereotype != "enum") && (stereotype != "typedef")) {
    QVector<UmlItem> children = this->children();
    unsigned u = children.size();
    
    while (u--)
      if ((children[u]->kind() != aNcRelation) &&
	  (((UmlClassItem *) children[u])->need_source()))
	return TRUE;
  }
  
  return FALSE;
}

# ifdef ROUNDTRIP

void UmlClass::upload(ClassContainer * cnt)
{
  UmlArtifact * art = associatedArtifact();
  QCString na;
  
  if (art != 0) {
    na = ((UmlPackage *) art->parent()->parent())->cppNamespace();
    if (! na.isEmpty())
      Namespace::set(na);
  }
    
  the_class = cnt->upload_define(this);
  
  const QVector<UmlItem> & ch = UmlItem::children();
  UmlItem ** v = ch.data();
  UmlItem ** const vsup = v + ch.size();
  
  for (;v != vsup; v += 1)
    (*v)->upload(the_class);
  
  if (! na.isEmpty())
    Namespace::unset();
}

bool UmlClass::set_roundtrip_expected() {
  if (is_roundtrip_expected() ||
      ((parent()->kind() != aClass) &&
       ((associatedArtifact() == 0) ||
	!associatedArtifact()->set_roundtrip_expected_for_class())))
    return TRUE;
  
  const QVector<UmlItem> & ch = UmlItem::children();
  UmlClassItem ** v = (UmlClassItem **) ch.data();
  UmlClassItem ** const vsup = v + ch.size();
  bool result = UmlClassItem::set_roundtrip_expected();
  
  for (;v != vsup; v += 1)
    result &= (*v)->set_roundtrip_expected();
  
  return result;
  
}

void UmlClass::mark_useless(QList<UmlItem> & l) {
  UmlClassItem::mark_useless(l);

  QVector<UmlItem> ch = UmlItem::children();
  UmlClassItem ** v = (UmlClassItem **) ch.data();
  UmlClassItem ** const vsup = v + ch.size();
    
  for (;v != vsup; v += 1)
    (*v)->mark_useless(l);
}

void UmlClass::scan_it(int & n) {
  if (is_roundtrip_expected())
    associatedArtifact()->scan_it(n);
}

void UmlClass::send_it(int n) {
  if (is_roundtrip_expected())
    associatedArtifact()->send_it(n);
}

UmlItem * UmlClass::search_for_att_rel(const QCString & name) {
  const QVector<UmlItem> & ch = UmlItem::children();
  UmlItem ** v = ch.data();
  UmlItem ** const vsup = v + ch.size();
  
  for (;v != vsup; v += 1) {
    switch ((*v)->kind()) {
    case anAttribute:
      if ((*v)->name() == name)
	return *v;
      break;
    case aRelation:
      if (((UmlRelation *) *v)->roleName() == name)
	return *v;
      break;
    default:
      break;
    }
  }

  return 0;
}

UmlExtraClassMember *
  UmlClass::search_for_extra(const QCString & name, const QCString & decl) {
  const QVector<UmlItem> & ch = UmlItem::children();
  UmlItem ** v = ch.data();
  UmlItem ** const vsup = v + ch.size();
  
  for (;v != vsup; v += 1) {
    if (((*v)->kind() == anExtraClassMember) &&
	((*v)->name() == name) &&
	!neq(((UmlExtraClassMember *) *v)->cppDecl(), decl))
      return (UmlExtraClassMember *) *v;
  }

  return 0;
}

UmlRelation * UmlClass::search_for_inherit(UmlClass * mother) {
  const QVector<UmlItem> & ch = UmlItem::children();
  UmlItem ** v = ch.data();
  UmlItem ** const vsup = v + ch.size();
  
  for (;v != vsup; v += 1) {
    if (((*v)->kind() == aRelation) &&
	(((UmlRelation *) *v)->roleType() == mother)) {
      switch (((UmlRelation *) *v)->relationKind()) {
      case aGeneralisation:
      case aRealization:
	return (UmlRelation *) *v;
      default:
	break;
      }
    }
  }

  return 0;
}

void UmlClass::reorder(QList<UmlItem> & expected_order) {
  if (expected_order.isEmpty())
    return;
  
  QVector<UmlItem> ch = UmlItem::children(); // copy
  UmlItem ** v = ch.data();
  
  unload(); // to not reload children each time

  //bool updated = FALSE;
  UmlItem * expected_previous = 0;
  QListIterator<UmlItem> expected_it(expected_order);
  UmlItem * expected;
  
  while ((expected = expected_it.current()) != 0) {
    if (*v != expected) {
      //updated = TRUE;
      expected->moveAfter(expected_previous);
      
      UmlItem * x1 = expected;
      
      do {
	UmlItem * x2 = *v;
	
	*v = x1;
	x1 = x2;
      } while (x1 != expected);
    }
    
    expected_previous = expected;
    ++expected_it;
    v += 1;
  }
  
  //if (updated)
  //  get_class()->set_updated();
}

# endif

#endif
