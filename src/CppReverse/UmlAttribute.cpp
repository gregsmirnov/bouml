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

#include "UmlAttribute.h"
#include "UmlPackage.h"
#include "Class.h"
#include "UmlClass.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "Lex.h"
#ifdef REVERSE
#include "Statistic.h"
# ifdef ROUNDTRIP
# include "UmlRelation.h"
# endif
#endif

#ifdef ROUNDTRIP
UmlAttribute * UmlAttribute::search_attr(UmlClass * cl, const QCString & name)
{
  UmlItem * x = cl->search_for_att_rel(name);
  
  if (x == 0)
    return 0;
  else if (x->kind() == anAttribute)
    return (UmlAttribute *) x;
  
  switch (((UmlRelation *) x)->relationKind()) {
  case aDirectionalAssociation:
  case aDirectionalAggregation:
  case aDirectionalAggregationByValue:
    x->deleteIt();
    break;
  case anAssociation:
  case anAggregation:
  case anAggregationByValue:
    if (x == ((UmlRelation *) x)->side(TRUE))
      ((UmlRelation *) x)->side(FALSE)->set_unidir();
    else
      ((UmlRelation *) x)->set_unidir();
    break;
  default:
    break;
  }

  return 0;
}

#endif

bool UmlAttribute::new_one(Class * container, const QCString & name,
			   const QCString & type, const QCString & modifier,
			   const QCString & pretype, const QCString & array,
			   aVisibility visibility, bool staticp, bool constp,
			   bool typenamep, bool mutablep, bool volatilep,
			   const QCString & bitfield, const QCString & value,
			   QCString comment, QCString description
#ifdef ROUNDTRIP
			   , bool roundtrip, QList<UmlItem> & expected_order
#endif
			   )
{
#ifdef DEBUG_BOUML
  cout << "ATTRIBUTE '" << name << "' type '" << type << "' modifier '" << modifier << "' array '" << array << "'\n";
#endif
  
  if (
#ifdef REVERSE
      container->from_libp() &&
#endif
      (visibility == PrivateVisibility)) {
    Lex::finish_line();
    Lex::clear_comments();
    return TRUE;
  }
  
  UmlClass * cl = container->get_uml();
  UmlAttribute * at;
  
#ifdef ROUNDTRIP
  bool created;
  
  if (!roundtrip ||
      ((at = search_attr(cl, name)) == 0)) {
#endif
    at = UmlBaseAttribute::create(cl, name);
    
    if (at == 0) {
      UmlCom::trace(QCString("<font face=helvetica><b>cannot add attribute <i>")
		    + name + "</i> in <i>" + QCString(cl->name())
		    + "</i></b></font><br><hr>");  
      return FALSE;
    }
  
#ifdef REVERSE
# ifndef ROUNDTRIP
    Statistic::one_attribute_more();
# else
    if (roundtrip)
      container->set_updated();
    created = TRUE;
  }
  else
    created = FALSE;
# endif
#endif
  
  Lex::finish_line();
  
  comment = Lex::get_comments(comment);
  description = Lex::get_description(description);
  
  bool pfunc = (type.find('$') != -1);
  UmlTypeSpec typespec;
  QCString typeform;
  QCString stereotype;
  
  if (! pfunc) {
    typeform = (pretype.isEmpty())
      ? QCString("${type}")
      : pretype + " ${type}";
    
    container->compute_type(type, typespec, typeform);
  }
  else {
    typespec.explicit_type = type.simplifyWhiteSpace();
    
    int index = typespec.explicit_type.find("${name}");
    
    if (index != -1)
      typespec.explicit_type.remove(index, 7);
  }
  
  QCString decl = CppSettings::attributeDecl("");
  int index = decl.find("${type}");
  
  if ((index == -1) ||
      (decl.find("${const}") == -1) ||
      (decl.find("${name}") == -1) ||
      (decl.find("${mutable}") == -1) ||
      (decl.find("${volatile}") == -1) ||
      (decl.find(';') == -1)) {
    decl = "    ${comment}${static}${mutable}${volatile}${const}${type} ${name}${value};";
    index = decl.find("${type}");
  }
  
  if (pfunc)
    decl.replace(index, decl.find("${name}") + 7 - index, type);
  else {
    if (!modifier.isEmpty())
      decl.insert(index + 7, QCString(" ") + modifier);
        
    if (typeform != "${type}")
      decl.replace(index, 7, typeform);
    else if (typespec.type == 0) {
      QCString t = typespec.explicit_type;
      int index2;
      
      if (!t.isEmpty() &&
	  (t.at(t.length() - 1) == '>') &&
	  ((index2 = t.find('<')) > 0)) {
	stereotype = t.left(index2);
	typespec.explicit_type =
	  // may be a,b ...
	  t.mid(index2 + 1, t.length() - 2 - index2);
	decl.replace(index, 7, "${stereotype}<${type}>");
      }
    }
    
    if (!array.isEmpty())
      decl.insert(decl.find("${name}") + 7, "${multiplicity}");
    
    if (!bitfield.isEmpty())
      decl.insert(decl.find(';'), QCString(" : ") + bitfield);
  }
  
  if (typenamep) {
    int index = decl.find("${const}") + 8; // find cannot return -1
    int index2 = decl.find("${mutable}") + 10; // find cannot return -1
    int index3 = decl.find("${volatile}") + 11; // find cannot return -1
    
    if (index2 > index) index = index2;
    if (index3 > index) index = index3;
    
    decl.insert(index, "typename ");
  }
  
  if (!value.isEmpty() && ((index = decl.find("${value}")) != -1))
    decl.insert(index + 2,  "h_");
  
#ifdef ROUNDTRIP
  if (roundtrip && !created) {
    if (decl.find("${description}") != -1) {
      if (nequal(at->description(), description)) {
	at->set_Description(description);
	container->set_updated();
      }
    }
    else if (nequal(at->description(), Lex::simplify_comment(comment))) {
      at->set_Description(comment); // comment was set
      container->set_updated();
    }
          
    if (at->isReadOnly() != constp) {
      at->set_isReadOnly(constp);
      container->set_updated();
    }
    
    if (at->isCppMutable() != mutablep) {
      at->set_isCppMutable(mutablep);
      container->set_updated();
    }
    
    if (at->isVolatile() != volatilep) {
      at->set_isVolatile(volatilep);
      container->set_updated();
    }
    
    if (at->isClassMember() != staticp) {
      at->set_isClassMember(staticp);
      container->set_updated();
    }
    
    if (neq(at->multiplicity(), array)) {
      at->set_Multiplicity(array);
      container->set_updated();
    }
    
    if (!staticp) {
      QCString v = at->defaultValue();
      
      if (!v.isEmpty() && (((const char *) v)[0] == '='))
	v = v.mid(1);
      
      if (nequal(v, value)) {
	at->set_DefaultValue(value);
	container->set_updated();
      }
    }
    
    if (at->visibility() != visibility) {
      at->set_Visibility(visibility);
      container->set_updated();
    }
    
    if (!stereotype.isEmpty()) {
      QCString cppst;
      
      if (!at->stereotype().isEmpty())
	cppst = CppSettings::relationAttributeStereotype(at->stereotype());
      
      if (cppst != stereotype) {
	at->set_Stereotype(stereotype);
	container->set_updated();
      }
    }
    
    if (!at->type().equal(typespec)) {
      at->set_Type(typespec);
      container->set_updated();
    }
    
    if (neq(at->cppDecl(), decl)) {
      at->set_CppDecl(decl);
      container->set_updated();
    }
          
    at->set_usefull();
    
    expected_order.append(at);
  }
  else {
#endif
    if (!comment.isEmpty())
      at->set_Description((decl.find("${description}") != -1)
			  ? description : Lex::simplify_comment(comment));
    
    if (constp)
      at->set_isReadOnly(TRUE);
    
    if (mutablep)
      at->set_isCppMutable(TRUE);
    
    if (volatilep)
      at->set_isVolatile(TRUE);
    
    if (staticp)
      at->set_isClassMember(TRUE);
    
    if (!array.isEmpty())
      at->set_Multiplicity(array);
    
    if (! value.isEmpty())
      at->set_DefaultValue(value);
    
    at->set_Visibility(visibility);
    
    if (! stereotype.isEmpty())
      at->set_Stereotype(stereotype);
    
    at->set_Type(typespec);
    
    at->set_CppDecl(decl);
    
#ifdef ROUNDTRIP
    if (roundtrip)
      expected_order.append(at);
  }
#endif
  
  return TRUE;
}
