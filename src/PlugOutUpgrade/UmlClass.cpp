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

#include "UmlClass.h"
#include "UmlClassView.h"
#include "UmlDeploymentView.h"
#include "UmlRelation.h"
#include "UmlArtifact.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "UmlExtraClassMember.h"
#include "util.h"

void UmlClass::made(UmlClassView * base_class_view, UmlClassView * user_class_view,
		    UmlDeploymentView * base_depl_view, UmlDeploymentView * user_depl_view,
		    UmlClass * base_item, QCString s, UmlClass *& base,
		    UmlClass *& user, UmlClass * user_interf) {
  unsigned uid = UmlCom::user_id();
  UmlRelation * rel;
  
  UmlCom::set_user_id(0);
  base = made(base_class_view, base_depl_view, "UmlBase" + s);
  base->set_isAbstract(TRUE);
  
  if ((rel = UmlBaseRelation::create(aGeneralisation, base, this)) == 0) {
    QCString msg = base->name() + " can't inherit " + name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
  
  if ((rel = UmlBaseRelation::create(aDependency, base_item, base)) == 0) {
    QCString msg = "Cannot add a friend dependency from " + base_item->name() + " to " + base->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_Stereotype("friend");
    rel->set_CppDecl("Generated");
    rel->set_JavaDecl("");
  }
  
  UmlCom::set_user_id(uid);
  
  user = made(user_class_view, user_depl_view, "Uml" + s);
  if ((rel = UmlBaseRelation::create(aGeneralisation, user, base)) == 0) {
    QCString msg = user->name() + " can't inherit " + base->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
  
  if (user_interf) {
    if ((rel = UmlBaseRelation::create(aGeneralisation, user, user_interf)) == 0) {
      QCString msg = user->name() + " can't inherit " + user_interf->name() + "<br>\n";
      
      UmlCom::trace(msg);
      throw 0;
    }
    else {
      rel->set_CppDecl("${type}");
      rel->set_JavaDecl("${type}");
    }
  }
}

void UmlClass::made(UmlClassView * base_class_view, UmlClassView * user_class_view,
		    UmlArtifact * base_art, UmlArtifact * user_art,
		    UmlClass * base_item, QCString s, UmlClass *& base,
		    UmlClass *& user, UmlClass * user_interf) {
  unsigned uid = UmlCom::user_id();
  UmlRelation * rel;
  
  UmlCom::set_user_id(0);
  base = made(base_class_view, base_art, "UmlBase" + s);
  base->set_isAbstract(TRUE);
  
  if ((rel = UmlBaseRelation::create(aGeneralisation, base, this)) == 0) {
    QCString msg = base->name() + " can't inherit " + name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
  
  if ((rel = UmlBaseRelation::create(aDependency, base_item, base)) == 0) {
    QCString msg = "Cannot add a friend dependency from " + base_item->name() + " to " + base->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_Stereotype("friend");
    rel->set_CppDecl("Generated");
    rel->set_JavaDecl("");
  }
  
  UmlCom::set_user_id(uid);
  
  user = made(user_class_view, user_art, "Uml" + s);
  if ((rel = UmlBaseRelation::create(aGeneralisation, user, base)) == 0) {
    QCString msg = user->name() + " can't inherit " + base->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  else {
    rel->set_CppDecl("${type}");
    rel->set_JavaDecl("${type}");
  }
  
  if (user_interf) {
    if ((rel = UmlBaseRelation::create(aGeneralisation, user, user_interf)) == 0) {
      QCString msg = user->name() + " can't inherit " + user_interf->name() + "<br>\n";
      
      UmlCom::trace(msg);
      throw 0;
    }
    else {
      rel->set_CppDecl("${type}");
      rel->set_JavaDecl("${type}");
    }
  }
}

UmlClass * UmlClass::made(UmlClassView * class_view,
			  UmlDeploymentView * depl_view,
			  QCString s, bool interf)
{
  UmlClass * cl = UmlBaseClass::create(class_view, s);
  
  if (cl == 0) {
    QCString msg = "can't create class '" + s + "' in " + class_view->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  if (interf) {
    UmlCom::trace("add interface " + s + "<br>\n");
    cl->set_Stereotype("interface");
    cl->set_JavaDecl(JavaSettings::interfaceDecl());
  }
  else {
    UmlCom::trace("add class " + s + "<br>\n");
    cl->set_JavaDecl(JavaSettings::classDecl());
  }
    
  cl->set_CppDecl(CppSettings::classDecl());
  
  UmlArtifact::made(depl_view, s)->addAssociatedClass(cl);
  
  return cl;
}

UmlClass * UmlClass::made(UmlClassView * class_view,
			  UmlArtifact * art,
			  QCString s, bool interf)
{
  UmlClass * cl = UmlBaseClass::create(class_view, s);
  
  if (cl == 0) {
    QCString msg = "can't create class '" + s + "' in " + class_view->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  if (interf) {
    UmlCom::trace("add interface " + s + "<br>\n");
    cl->set_Stereotype("interface");
    cl->set_JavaDecl(JavaSettings::interfaceDecl());
  }
  else {
    UmlCom::trace("add class " + s + "<br>\n");
    cl->set_JavaDecl(JavaSettings::classDecl());
  }
    
  cl->set_CppDecl(CppSettings::classDecl());
  
  art->addAssociatedClass(cl);
  
  return cl;
}

void UmlClass::add_kind(const char * k) {
  UmlOperation * op = UmlOperation::create(this, "kind");
  
  if (op == 0) {
    QCString msg = "can't add operation 'kind' in " + name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  UmlCom::trace("add operation " + name() + "::kind" + "<br>\n");
  
  op->set_Description(" returns the kind of the item");  
  
  UmlTypeSpec t;
  
  t.type = UmlClass::get("anItemKind", 0);
  
  op->set_ReturnType(t);
  op->remove_cpp_throw();
  op->set_isCppVirtual(TRUE);
  op->set_CppBody(QCString("  return ") + k + ";");
  
  op->set_JavaDef(JavaSettings::operationDef());
  op->set_JavaBody(QCString("  return anItemKind.") + k + ";");
}

void UmlClass::add_default_base_op(UmlClass * super, UmlClass * user,
				   UmlClass * parent, QCString s,
				   const char * k, bool unnamed) {
  unsigned uid = UmlCom::user_id();
  
  UmlCom::set_user_id(0);
  
  UmlOperation * op = add_op("create", PublicVisibility, user, TRUE);
  
  if (op == 0) {
    QCString msg = "can't add operation 'create' in " + name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  op->set_isClassMember(TRUE);
  op->add_param(0, InputOutputDirection, "parent", parent);
  if (unnamed) {
    op->set_Description(" returns a new " + s + " created under 'parent'\n\
\n\
 In case it cannot be created ('parent' cannot contain it etc ...)\n\
 return 0 in C++ and produce a RuntimeException in Java\n");
    op->set_cpp("${type} *", 
		"${t0} * ${p0}",
		"  return (" + user->name() + " *) parent->create_(" + k + ", \"\");\n",
		FALSE, 0, 0);
    op->set_java("${type}", "${t0} ${p0}",
		 "  return (" + user->name() + ") parent.create_(anItemKind." + k + ", \"\");\n",
		 FALSE);
  }
  else {
    op->set_Description("  returns a new " + s + " named 's' created under 'parent'\n\
\n\
 In case it cannot be created (the name is already used or\n\
 invalid, 'parent' cannot contain it etc ...) return 0 in C++\n\
 and produce a RuntimeException in Java");
    op->add_param(1, InputOutputDirection, "s", "str");
    op->set_cpp("${type} *", 
		"${t0} * ${p0}, const ${t1} ${p1}",
		"  return (" + user->name() + " *) parent->create_(" + k + ", s);\n",
		FALSE, 0, 0);
    op->set_java("${type}", "${t0} ${p0}, ${t1} ${p1}",
		 "  return (" + user->name() + ") parent.create_(anItemKind." + k + ", s);\n",
		 FALSE);
  }
  
  add_kind(k);
  add_constr(super, ProtectedVisibility, FALSE);
  UmlCom::set_user_id(uid);
}

UmlOperation * UmlClass::add_constr(UmlClass * super, aVisibility v, bool unnamed) {
  UmlOperation * op = UmlOperation::create(this, name());

  if (op == 0) {
    QCString msg = "can't add constructor in " + name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  UmlCom::trace("add operation " + name() + "::" + name() + "<br>\n");
  
  op->remove_cpp_throw();
  op->set_Visibility(v);
  op->set_Description("  the constructor, do not call it yourself !!!!!!!!!!");
  
  if (super != 0) {
    op->add_param(0, InputDirection, "id", "item_id");
    if (unnamed) {
      op->set_cpp(": " + super->name() + "(id, \"\")", 
		  "${t0} ${p0}",
		  "", TRUE, 0, 0);
      op->set_java("", 
		   "${t0} ${p0}",
		   "  super(id, \"\");\n", TRUE);
    }
    else {
      op->add_param(1, InputDirection, "s", "string");
      op->set_cpp(": " + super->name() + "(id, s)", 
		  "${t0} ${p0}, const ${t1} & ${p1}",
		  "", TRUE, 0, 0);
      op->set_java("", 
		   "${t0} ${p0}, ${t1} ${p1}",
		   "  super(id, s);\n", TRUE);
    }
  }
  
  return op;
}

UmlOperation * UmlClass::add_destr(aVisibility v, const char * comment) {
  QCString s = "~" + name();
  UmlOperation * op = UmlOperation::create(this, s);

  if (op == 0) {
    QCString msg = "can't add destructor in " + name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  UmlCom::trace("add operation " + name() + "::~" + name() + "<br>\n");
  
  op->remove_cpp_throw();
  op->set_Visibility(v);
  if (comment != 0)
    op->set_Description(comment);
  op->set_cpp("", "", "", FALSE, 0, 0);
  op->set_JavaDecl("");
  
  return op;
}


UmlOperation * UmlClass::add_op(const char * name, aVisibility v,
				UmlClass * return_type, bool excpt) {
  UmlTypeSpec t;
  
  t.type = return_type;
  return add_op(name, v, t, excpt);
}

UmlOperation * UmlClass::add_op(const char * name, aVisibility v,
				const char * return_type, bool excpt) {
  UmlTypeSpec t;
  
  t.explicit_type = return_type;
  return add_op(name, v, t, excpt);
}

UmlOperation * UmlClass::add_op(const char * name, aVisibility v,
				UmlTypeSpec & return_type, bool excpt) {
  UmlOperation * op = UmlOperation::create(this, name);

  if (op == 0) {
    QCString msg =
      QCString("can't add operation '") + name + "' in " + this->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  UmlCom::trace("add operation " + this->name() + "::" + name + "<br>\n");
  
  op->remove_cpp_throw();
  op->set_ReturnType(return_type);
  op->set_Visibility(v);
  
  if (excpt) {
    UmlTypeSpec t;
    
    t.explicit_type = "RuntimeException";
    op->addException(0, t);
  }
  
  return op;
}

UmlAttribute * UmlClass::add_attribute(const char * name, aVisibility v,
				       const char * type, const char * if_def,
				       const char * end_if, const char * bitfield) {
  UmlTypeSpec t;
  
  t.explicit_type = type;
  return add_attribute(name, v, t, if_def, end_if, bitfield);
}

UmlAttribute * UmlClass::add_attribute(const char * name, aVisibility v,
				       UmlClass * type, const char * if_def,
				       const char * end_if, const char * bitfield) {
  UmlTypeSpec t;
  
  t.type = type;
  return add_attribute(name, v, t, if_def, end_if, bitfield);
}

UmlAttribute * UmlClass::add_attribute(const char * name, aVisibility v,
				       UmlTypeSpec & type, const char * if_def,
				       const char * end_if, const char * bitfield) {
  UmlAttribute * ar = UmlAttribute::create(this, name);

  if (ar == 0) {
    QCString msg =
      QCString("can't add attribute '") + name + "' in " + this->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  UmlCom::trace("add attribute " + this->name() + "::" + name + "<br>\n");
  
  ar->set_Type(type);
  ar->set_Visibility(v);
  
  QCString s = CppSettings::attributeDecl("");
  
  if (bitfield != 0) {
    int index = s.find("${type} ${name}");
    
    if (index != -1)
      s.insert(index + 15, bitfield);
  }
  conditional(s, if_def, end_if);
  ar->set_CppDecl(s);
  ar->set_JavaDecl(JavaSettings::attributeDecl(""));
  
  return ar;
}

UmlAttribute * UmlClass::add_enum_item(const char * name) {
  UmlAttribute * at = UmlBaseAttribute::create(this, name);
    
  if (at == 0) {
    UmlCom::trace("cannot add enum item " + this->name() + "::" + name + "<br>\n");
    throw 0;
  }
  else {
    UmlCom::trace("add enum item " + this->name() + "::" + name + "<br>\n");
    at->set_CppDecl(CppSettings::enumItemDecl());
    at->set_JavaDecl(JavaSettings::enumPatternItemDecl());
    at->set_Visibility(PublicVisibility);
  }
  
  return at;
}

UmlRelation * UmlClass::add_relation(aRelationKind k, const char * name,
				     aVisibility v, UmlClass * type,
				     const char * if_def, const char * end_if,
				     const char * bitfield)
{
  UmlRelation * rel = UmlRelation::create(k, this, type);
  
  if (rel == 0) {
    QCString msg =
      QCString("can't add relation '") + name + "' in " + this->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  UmlCom::trace("add relation " + this->name() + "::" + name + "<br>\n");
  
  rel->set_RoleName(name);
  rel->set_Visibility(v);
  
  bool byValue;
  
  switch (k) {
  case anAggregationByValue:
  case aDirectionalAggregationByValue:
    byValue = TRUE;
    break;
  default:
    byValue = FALSE;
  }
  
  QCString s = CppSettings::relationDecl(byValue, "");
  
  if (bitfield != 0) {
    int index = s.find("${type} ${name}");
    
    if (index != -1)
      s.insert(index + 15, bitfield);
  }
  
  conditional(s, if_def, end_if);
  rel->set_CppDecl(s);
  rel->set_JavaDecl(JavaSettings::relationDecl(""));
  
  return rel;
}

UmlRelation * UmlClass::add_vect_assoc(const char * name, aVisibility v, UmlClass * type,
				       const char * if_def, const char * end_if)
{
  UmlRelation * rel = UmlRelation::create(aDirectionalAggregationByValue, this, type);
  
  if (rel == 0) {
    QCString msg =
      QCString("can't add relation '") + name + "' in " + this->name() + "<br>\n";
    
    UmlCom::trace(msg);
    throw 0;
  }
  
  UmlCom::trace("add relation " + this->name() + "::" + name + "<br>\n");
  
  rel->set_RoleName(name);
  rel->set_Visibility(v);
  rel->set_Multiplicity("*");
  rel->set_Stereotype("vector");
  
  QCString s;
  
  s = CppSettings::relationDecl(TRUE, "");
  s.replace(s.find("${type}"), 7, "QVector<${type}>");
  conditional(s, if_def, end_if);
  rel->set_CppDecl(s);
  
  s = JavaSettings::relationDecl("");
  s.insert(s.find("${type}") + 7, "[]");
  rel->set_JavaDecl(s);
  
  return rel;
}

UmlOperation * UmlClass::get_operation(const char * who) {
  const QVector<UmlItem> ch = children();
  
  for (unsigned i = 0; i != ch.size(); i += 1)
    if ((ch[i]->kind() == anOperation) &&
	(ch[i]->name() == who))
      return (UmlOperation *) ch[i];
    
  return 0;
}

UmlAttribute * UmlClass::get_attribute(const char * who) {
  const QVector<UmlItem> ch = children();
  
  for (unsigned i = 0; i != ch.size(); i += 1)
    if ((ch[i]->kind() == anAttribute) &&
	(ch[i]->name() == who))
      return (UmlAttribute *) ch[i];
    
  return 0;
}

UmlRelation * UmlClass::get_relation(aRelationKind k, const char * who) {
  const QVector<UmlItem> ch = children();
  
  for (unsigned i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == aRelation) {
      UmlRelation * rel = (UmlRelation *) ch[i];

      if ((rel->relationKind() == k) && 
	  ((who == 0) || (rel->roleName() == who)))
	return rel;
    }
  }
    
  return 0;
}

UmlRelation * UmlClass::get_relation(const char * who) {
  const QVector<UmlItem> ch = children();
  
  for (unsigned i = 0; i != ch.size(); i += 1) {
    if (ch[i]->kind() == aRelation) {
      UmlRelation * rel = (UmlRelation *) ch[i];

      if (rel->roleName() == who)
	return rel;
    }
  }
    
  return 0;
}

void UmlClass::replace_friend() {
  QCString s;
  const QVector<UmlItem> ch = children();
  unsigned i = ch.size();
  
  while (i--) {
    if ((ch[i]->kind() == aRelation) &&
	(((UmlRelation *) ch[i])->relationKind() == aDependency) &&
	(ch[i]->stereotype() == "friend")) {
      s += "  friend class " + ((UmlRelation *) ch[i])->roleType()->name()
	+ ";\n";
      ch[i]->deleteIt();
    }
  }
  
  if (!s.isEmpty())
    UmlExtraClassMember::create(this, "friend")->set_CppDecl(s);
}

void UmlClass::add_friend(const char * scl) {
  QCString s = QCString("  friend class ") + scl + ";\n";
  const QVector<UmlItem> ch = children();
  unsigned i = ch.size();
  
  while (i--) {
    if ((ch[i]->kind() == anExtraClassMember) &&
	(ch[i]->name() == "friend")) {
      UmlExtraClassMember * x = (UmlExtraClassMember *) ch[i];
      QCString f = x->cppDecl() + s;
      
      x->set_CppDecl(f);
      return;
    }
  }
  
  UmlExtraClassMember::create(this, "friend")->set_CppDecl(s);
}
