
#include "UmlBaseRelation.h"
#include "UmlRelation.h"
#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"

#include "UmlCom.h"
UmlRelation * UmlBaseRelation::create(aRelationKind kind, UmlClass * start, UmlClass * end)
{
  UmlCom::send_cmd(start->_identifier, createCmd, aRelation,
		   kind, end->_identifier);
  UmlRelation * result = (UmlRelation *) UmlBaseItem::read_();
  
  if (result != 0) {
    start->reread_children_if_needed_();
    
    switch (kind) {
    case anAssociation:
    case anAggregation:
    case anAggregationByValue:
      // bi-directional
      end->reread_children_if_needed_();
      break;
    case aGeneralisation:
    case aRealization:
      if (!end->formals().isEmpty())
	// to update actuals & base_type
	start->reread_if_needed_();
    default:
      break;
    }
  }
  
  return result;
}

anItemKind UmlBaseRelation::kind() {
  return aRelation;
}

aRelationKind UmlBaseRelation::relationKind() {
  read_if_needed_();
  
  return _rel_kind;
}

UmlRelation * UmlBaseRelation::side(bool first) {
  UmlCom::send_cmd(_identifier, sideCmd, (char) first);
  
  return (UmlRelation *) UmlBaseItem::read_();
}

bool UmlBaseRelation::isReadOnly() {
  read_if_needed_();
  
  return _read_only;
}

bool UmlBaseRelation::set_isReadOnly(bool y) {
  UmlCom::send_cmd(_identifier, setIsReadOnlyCmd, (char) y);
  if (UmlCom::read_bool()) {
    _read_only = y;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & UmlBaseRelation::defaultValue() {
  read_if_needed_();
  
  return _default_value;
}

bool UmlBaseRelation::set_DefaultValue(const char * s) {
  return set_it_(_default_value, s, setDefaultValueCmd);
}

bool UmlBaseRelation::set_Stereotype(const QCString & s) {
  if (UmlBaseItem::set_Stereotype(s)) {
    UmlBaseRelation * other = (UmlBaseRelation *) UmlBaseItem::read_();
    
    if (other != 0)
      other->_stereotype = s;
    return TRUE;
  }
  else
    return FALSE;
}

UmlClass * UmlBaseRelation::roleType() {
  read_if_needed_();
  
  return _role_type;
}

UmlTypeSpec UmlBaseRelation::association() {
  read_if_needed_();
  
  return _association;
}

bool UmlBaseRelation::set_Association(const UmlTypeSpec & t) {
  if (set_it_(_association, t, setRelationAssocClassCmd)) {
    UmlBaseRelation * other = (UmlBaseRelation *) UmlBaseItem::read_();
    
    if (other != 0)
      other->_association = t;
    return TRUE;
  }
  else
    return FALSE;
}

const QCString & UmlBaseRelation::roleName() {
  read_if_needed_();
  
  return _role_name;
}

bool UmlBaseRelation::set_RoleName(const char * s) {
  return set_it_(_role_name, s, setRoleNameCmd);
}

const QCString & UmlBaseRelation::multiplicity() {
  read_if_needed_();
  
  return _multiplicity;
}

bool UmlBaseRelation::set_Multiplicity(const char * s) {
  return set_it_(_multiplicity, s, setMultiplicityCmd);
}

UmlOperation * UmlBaseRelation::getOperation() {
  read_if_needed_();
  
  return _get_oper;
}

bool UmlBaseRelation::addGetOperation() {
  UmlCom::send_cmd(_identifier, addGetOperCmd);
  if (UmlCom::read_bool()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

UmlOperation * UmlBaseRelation::setOperation() {
  read_if_needed_();
  
  return _set_oper;
}

bool UmlBaseRelation::addSetOperation() {
  UmlCom::send_cmd(_identifier, addSetOperCmd);
  if (UmlCom::read_bool()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

#ifdef WITHCPP
bool UmlBaseRelation::cppVirtualInheritance() {
  read_if_needed_();
  
  return _cpp_virtual_inheritance;
}

bool UmlBaseRelation::set_CppVirtualInheritance(bool y) {
  bool b;
  
  if (set_it_(b, y, setCppVirtualInheritanceCmd)) {
    _cpp_virtual_inheritance = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseRelation::isCppMutable() {
  read_if_needed_();
  
  return _cpp_mutable;
}

bool UmlBaseRelation::set_isCppMutable(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsCppMutableCmd)) {
    _cpp_mutable = y;
    return TRUE;
  }
  else
    return FALSE;
}
#endif

#ifdef WITHJAVA
bool UmlBaseRelation::isJavaTransient() {
  read_if_needed_();
  
  return _java_transient;
}

bool UmlBaseRelation::set_isJavaTransient(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsJavaTransientCmd)) {
    _java_transient = y;
    return TRUE;
  }
  else
    return FALSE;

}
#endif

#ifdef WITHIDL
bool UmlBaseRelation::isIdlTruncatableInheritance() {
  read_if_needed_();
  
  return _idl_truncatable;
}

bool UmlBaseRelation::set_isIdlTruncatableInheritance(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsIdlTruncatableCmd)) {
    _idl_truncatable = y;
    return TRUE;
  }
  else
    return FALSE;
}

QCString UmlBaseRelation::idlCase() {
  read_if_needed_();
  
  return (_idl_case != 0) ? _idl_case->name() : _idl_explicit_case;
}

bool UmlBaseRelation::set_IdlCase(UmlAttribute * a) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, a->_identifier, "");
  if (UmlCom::read_bool()) {
    _idl_case = a;
    _idl_explicit_case = 0;
    return TRUE;
  }
 else
   return FALSE;
}

bool UmlBaseRelation::set_IdlCase(const char * s) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, (void *) 0, s);
  if (UmlCom::read_bool()) {
    _idl_case = 0;
    _idl_explicit_case = s;
    return TRUE;
  }
 else
   return FALSE;
}
#endif

void UmlBaseRelation::unload(bool rec, bool del) {
  _role_name = 0;
  _multiplicity = 0;
  _default_value = 0;
#ifdef WITHIDL
  _idl_explicit_case = 0;
#endif
  UmlBaseClassMember::unload(rec, del);
}

void UmlBaseRelation::read_uml_() {
  UmlBaseClassMember::read_uml_();
  _rel_kind = (aRelationKind) UmlCom::read_char();
  _role_type = (UmlClass *) UmlBaseItem::read_();
  _association.type = (UmlClass *) UmlBaseItem::read_();
  if (_association.type == 0)
    _association.explicit_type = UmlCom::read_string();
  _role_name = UmlCom::read_string();
  _multiplicity = UmlCom::read_string();
  _default_value = UmlCom::read_string();
  _read_only = UmlCom::read_bool();
#if APIVERSION > 41
  /*_is_derived =*/ UmlCom::read_bool();
  /*_is_derivedunion =*/ UmlCom::read_bool();
  /*_is_ordered =*/ UmlCom::read_bool();
  /*_is_unique =*/ UmlCom::read_bool();
#endif
  _get_oper = (UmlOperation *) UmlBaseItem::read_();
  _set_oper = (UmlOperation *) UmlBaseItem::read_();
}

#ifdef WITHCPP
void UmlBaseRelation::read_cpp_() {
  UmlBaseClassMember::read_cpp_();
  _cpp_mutable = UmlCom::read_bool();
  _cpp_virtual_inheritance = UmlCom::read_bool();
}
#endif

#ifdef WITHJAVA
void UmlBaseRelation::read_java_() {
  UmlBaseClassMember::read_java_();
  _java_transient = UmlCom::read_bool();
}
#endif

#ifdef WITHPHP
void UmlBaseRelation::read_php_() {
  UmlBaseClassMember::read_php_();
}
#endif

#ifdef WITHPYTHON
void UmlBaseRelation::read_python_() {
  UmlBaseClassMember::read_python_();
}
#endif

#ifdef WITHIDL
void UmlBaseRelation::read_idl_() {
  UmlBaseClassItem::read_idl_();
  _idl_truncatable = UmlCom::read_bool();
  _idl_case = (UmlAttribute *) UmlBaseItem::read_();
  if (_idl_case == 0)
    _idl_explicit_case = UmlCom::read_string();
}
#endif

// not in plug-out managed through bouml
// applied on first role
bool UmlBaseRelation::set_rel_kind(aRelationKind k) {
  UmlRelation * other = side(FALSE);
  
  UmlCom::send_cmd(_identifier, setTypeCmd, (char) k);
  
  if (UmlCom::read_bool()) {
    bool old_bi;
    bool new_bi;
    
    switch (_rel_kind) {
    case anAssociation:
    case anAggregation:
    case anAggregationByValue:
      old_bi = TRUE;
      break;
    default:
      old_bi = FALSE;
    }
    
    switch (k) {
    case anAssociation:
    case anAggregation:
    case anAggregationByValue:
      new_bi = TRUE;
      break;
    default:
      new_bi = FALSE;
    }
    
    if (new_bi != old_bi) {
      if (other != 0) {
	other->unload();
	delete other;
      }
      
      _role_type->reread_children_if_needed_();
    }
    
    _rel_kind = k;
    return TRUE;
  }
  else
    return FALSE;
}
