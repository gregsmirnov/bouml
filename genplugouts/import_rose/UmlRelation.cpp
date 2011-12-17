
#include "UmlRelation.h"
#include "UmlClass.h"
#include "File.h"

#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "IdlSettings.h"
void UmlRelation::Role::import(File & f) {
  f.read("(");
  f.read("object");
  f.read("Role");
  if (f.read(name) != STRING)
    f.syntaxError(name, "role name");
    if (*name == '$')
      // unnamed
      name = "";

  visibility = PublicVisibility;
  is_navigable = FALSE;
  is_aggregate = FALSE;
  is_static = FALSE;
  is_byvalue = FALSE;
  
  QCString ste; 
  QCString s;
  int k; 
     
  do { 
    k = f.readDefinitionBeginning(s, id, ste, doc, prop); 
  } while (id.isEmpty()); 
  
  for (;;) {
    switch (k) {
    case ')':
      target = 0;
      return;
    case ATOM:
      break;
    default:
      f.syntaxError(s);
    }
    
    // atom
    if (s == "quid") {
      if (f.read(id) != STRING)
	f.syntaxError(id, "quid value");
    }
    else if (s == "quidu") {
      if (f.read(target_id) != STRING)
	f.syntaxError(target_id, "quidu value");
      target = (UmlClass *) UmlItem::findItem(target_id, aClass);
    }
    else if (s == "client_cardinality") {
      f.read("(");
      f.read("value");
      f.read("cardinality");
      if (f.read(cardinality) != STRING)
	f.syntaxError(cardinality, "the cardinality");
      f.read(")");
    }
    else if (s == "exportControl")
      visibility = f.readVisibility();
    else if (s == "is_navigable") 
      is_navigable = f.readBool(); 
    else if (s == "is_aggregate") 
      is_aggregate = f.readBool(); 
    else if (s == "Containment") {
      if (f.read(s) != STRING)
        f.syntaxError(s, "containment value");
      is_byvalue = (s == "By Value");
    }
    else if (s == "static")
      is_static = f.readBool();
    else
      f.skipNextForm();

    k = f.read(s);
  }
}

aRelationKind UmlRelation::Role::rel_kind(bool bidir) {
  if (bidir) {
    if (is_byvalue)
      return anAggregationByValue;
    if (is_aggregate)
      return anAggregation;
    return anAssociation;
  }
  
  if (is_byvalue)
    return aDirectionalAggregationByValue;
  if (is_aggregate)
    return aDirectionalAggregation;
  return aDirectionalAssociation;
}

void UmlRelation::import(File & f)
{
  if (scanning)
    f.skipBlock();
  else {
    QCString s;
    
    if (f.read(s) != STRING)
      f.syntaxError(s, "relation's name");
    else if (*s == '$')
      // unnamed
      s = "";
    
    QCString id;
    QCString ste;
    QCString doc;
    QDict<QCString> prop;
    QCString s2;
    int k;
    
    do {
      k = f.readDefinitionBeginning(s2, id, ste, doc, prop);
    } while (id.isEmpty());
    
    for (;;) {
      if (k == ATOM) {
	if (s2 == "roles")
	  break;
	f.skipNextForm();
	k = f.read(s2);
      }
      else
	f.syntaxError(s);
    }
    
    f.read("(");
    f.read("list");
    f.read("role_list");
    
    Role role_1; 
    Role role_2; 
    Role * role1 = &role_1; 
    Role * role2 = &role_2; 
    
    role_1.import(f);
    role_2.import(f);
    
    UmlRelation * r = 0; 
    bool bidir = role_1.is_navigable && role_2.is_navigable;

    // place information in the logical side !
    bool b = role_1.is_aggregate;

    role_1.is_aggregate = role_2.is_aggregate;
    role_2.is_aggregate = b;

    role_1.is_aggregate |= role_1.is_byvalue;
    role_2.is_aggregate |= role_2.is_byvalue;

    if (bidir && role_2.is_aggregate) {
      // manage bouml limitation : only role1 may be an aggregate
      if (role_1.is_aggregate)
        bidir = FALSE;
      else {
	// exchange roles
	role1 = &role_2;
	role2 = &role_1;
      }
    }

    if ((role1->target != 0) && (role2->target != 0)) { 
      if (role1->is_navigable) {
	r = UmlRelation::create(role1->rel_kind(bidir), role2->target, role1->target);
	
	if (r == 0) {
	  UmlCom::trace("<br>cannot create relation '" + role1->name + "' from '" +
			role2->target->fullName() + "' to '" +
			role1->target->fullName() + "'");
	  f.read(")");
	  f.skipBlock();
	  return;
	}
	
	if (!ste.isEmpty())
	  r->set_Stereotype(ste);
	
	if (! s.isEmpty())
	  r->set_Name(s);
	
	r->import(role1);
      }
      
      if (role2->is_navigable) {
	if (bidir) {
	  const QVector<UmlItem> ch = role2->target->children();
	  
	  r = (UmlRelation *) ch.at(ch.count() - 1);
	}
	else {
	  r = UmlRelation::create(role2->rel_kind(FALSE),role1->target, role2->target);
	  
	  if (r == 0) {
	    UmlCom::trace("<br>cannot create relation '" + role2->name +
			  "' from '" +
			  role1->target->fullName() + "' to '" +
			  role2->target->fullName() + "'");
	    f.read(")");
	    f.skipBlock();
	    return;
	  }
	  
	  if (!ste.isEmpty())
	    r->set_Stereotype(ste);
	   
	  if (! s.isEmpty()) 
	    r->set_Name(s); 
	}
	  
	r->import(role2);
      }
    }

    f.read(")");
    f.skipBlock();
  }
}

void UmlRelation::import(UmlRelation::Role * role) {
  if (!role->doc.isEmpty())
    set_Description(role->doc);
  
  if (! role->cardinality.isEmpty())
    set_Multiplicity(role->cardinality);
  
  if (role->is_static)
    set_isClassMember(TRUE);
  
  if (!role->name.isEmpty())
    set_RoleName(role->name);
  
  set_Visibility(role->visibility);
  
  switch (((UmlClass *) parent())->language()) {
  case Cplusplus:
  case AnsiCplusplus:
  case VCplusplus:
    cplusplus(role);
    break;
  case Oracle8:
    oracle8(role);
    break;
  case Corba:
    corba(role);
    break;
  case Java:
    java(role);
    break;
  default:
    break;
  }
  
  setProperties(role->prop);
}

void UmlRelation::cplusplus(UmlRelation::Role * role) {
  set_CppDecl(CppSettings::relationDecl(role->is_byvalue,
					role->cardinality));
}

void UmlRelation::oracle8(UmlRelation::Role *) {
}

void UmlRelation::corba(UmlRelation::Role * role) {
  set_IdlDecl(IdlSettings::relationDecl(role->cardinality));
}

void UmlRelation::java(UmlRelation::Role * role) {
  QCString * v;
    
  if ((v = role->prop.find("Java/Final")) != 0) {
    if (*v == "TRUE")
      set_isReadOnly(TRUE);
    role->prop.remove("Java/Final");
  }

  if ((v = role->prop.find("Java/Transient")) != 0) {
    if (*v == "TRUE")
      set_isJavaTransient(TRUE);
    role->prop.remove("Java/Transient");
  }

  if ((v = role->prop.find("Java/Volatile")) != 0) {
    if (*v == "TRUE")
      set_isVolatile(TRUE);
    role->prop.remove("Java/Volatile");
  }

  role->prop.remove("Java/IsNavigable");

  set_JavaDecl(JavaSettings::relationDecl(role->cardinality));
}

