#ifndef _UMLBASERELATION_H
#define _UMLBASERELATION_H


#include "UmlClassMember.h"
#include "aRelationKind.h"
#include "anItemKind.h"
#include <qcstring.h>
#include "UmlTypeSpec.h"

class UmlRelation;
class UmlClass;
class UmlOperation;
class UmlAttribute;

// Manage the relations between classes
class UmlBaseRelation : public UmlClassMember {
  public:
    // returns a new relation of the given 'kind' from 'start' to 'end'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRelation * create(aRelationKind kind, UmlClass * start, UmlClass * end);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the kind of the relation
    aRelationKind relationKind();

    // if 'first' is true returns the relation associated to the
    // first role, else the relation associated to the second
    // role or 0/null if the relation is uni directional
    UmlRelation * side(bool first);

    // indicates if the relation is read only, returns TRUE if yes
    bool isReadOnly();

    // to set the 'read only' state of the relation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReadOnly(bool y);

    // return the  return the property 'derived'
    bool isDerived();

    // return the  return the property 'derived union'
    bool isDerivedUnion();

    // Set the properties 'derived' and 'union'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isDerived(bool is_derived, bool is_union);

    // return the  return the property 'ordered'
    bool isOrdered();

    // set the property 'ordered'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isOrdered(bool v);

    // return the  return the property 'unique'
    bool isUnique();

    // set the property 'unique'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isUnique(bool v);

    // returns the default relation value, may be an empty string
    const QCString & defaultValue();

    // to set the default relation value ("" allowed)
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_DefaultValue(const char * s);

    // to set the stereotype
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    // redefined in case the relation is bidirectional to set the stereotype
    // of the relation corresponding to the other direction
    virtual bool set_Stereotype(const QCString & s);

    // returns the 'end' class (the 'start' class is the parent of the relation) no set !
    UmlClass * roleType();

    // return the associated class/type, may be an empty spec
    UmlTypeSpec association();

    // set the associated class/type, may be an empty spec
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_Association(const UmlTypeSpec & t);

    // returns the name of the role
    const QCString & roleName();

    // to set the name of the role
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_RoleName(const char * s);

    // returns the multiplicity (may be an empty string)
    const QCString & multiplicity();

    // to set the multiplicity
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Multiplicity(const char * s);

    // returns the 'get' operation of the relation, or 0 if it does not exist
    UmlOperation * getOperation();

    // to generate an associated 'get' operation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool addGetOperation();

    // returns the 'set' operation of the relation, or 0 if it does not exist
    UmlOperation * setOperation();

    // to generate an associated 'set' operation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool addSetOperation();

#ifdef WITHCPP
    // indicates if the inheritance is virtual in C++, returns TRUE if yes
    bool cppVirtualInheritance();

    // to set if the inheritance is virtual in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppVirtualInheritance(bool y);

    // Indicate if the relation is 'mutable'
    bool isCppMutable();

    // Set if the relation is 'mutable'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isCppMutable(bool y);
#endif

#ifdef WITHJAVA
    // indicates if the relation is 'transient', returns TRUE if yes
    bool isJavaTransient();

    // to set the 'transient' state of the relation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isJavaTransient(bool y);
#endif

#ifdef WITHIDL
    // indicates if the relation is 'truncatable', returns TRUE if yes
    // have sense only for a valuetype inheritance
    bool isIdlTruncatableInheritance();

    // to set if the inheritance is 'truncatable'
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isIdlTruncatableInheritance(bool y);

    // in case the relation is an IDL union's member returns the
    // corresponding 'case', an empty string in case it is not specified
    QCString idlCase();

    // to set the 'case' even the relation is not (already) known as
    // an IDL union's member
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlCase(UmlAttribute * a);

    // to set the 'case' even the relation is not (already) known as
    // an IDL union's member
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlCase(const char * s);
#endif

    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    aRelationKind _rel_kind : 8;

    bool _read_only : 1;

    bool _derived : 1;

    bool _derived_union : 1;

    bool _ordered : 1;

    bool _unique : 1;

#ifdef WITHCPP
    bool _cpp_virtual_inheritance : 1;

    bool _cpp_mutable : 1;
#endif

#ifdef WITHJAVA
    bool _java_transient : 1;
#endif

#ifdef WITHIDL
    bool _idl_truncatable : 1;
#endif

    QCString _default_value;

    UmlClass * _role_type;

    QCString _role_name;

    QCString _multiplicity;

    UmlTypeSpec _association;

    UmlOperation * _get_oper;

    UmlOperation * _set_oper;

    // exclusive with idl_explicit_case
    UmlAttribute * _idl_case;

#ifdef WITHIDL
    QCString _idl_explicit_case;
#endif


  protected:
    //internal, do NOT use it
    
    virtual void read_uml_();

#ifdef WITHCPP
    //internal, do NOT use it
    
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    //internal, do NOT use it
    
    virtual void read_java_();
#endif

#ifdef WITHPHP
    //internal, do NOT use it
    
    virtual void read_php_();
#endif

#ifdef WITHPYTHON
    //internal, do NOT use it
    
    virtual void read_python_();
#endif

#ifdef WITHIDL
    //internal, do NOT use it
    
    virtual void read_idl_();
#endif

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseRelation(void * id, const QCString & n);

};

inline UmlBaseRelation::UmlBaseRelation(void * id, const QCString & n) : UmlClassMember(id, n) {
  _role_type = 0;
  _get_oper = 0;
  _set_oper = 0;
#ifdef WITHIDL
  _idl_case = 0;
#endif
}

#endif
