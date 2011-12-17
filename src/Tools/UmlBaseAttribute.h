#ifndef _UMLBASEATTRIBUTE_H
#define _UMLBASEATTRIBUTE_H


#include "UmlClassMember.h"
#include "anItemKind.h"
#include <qcstring.h>
#include "UmlTypeSpec.h"

class UmlAttribute;
class UmlClass;
class UmlOperation;

//  Manage the class's attributs

class UmlBaseAttribute : public UmlClassMember {
  public:
    // returns a new attribute named 'name' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAttribute * create(UmlClass * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // indicates if the attribute is read only, returns TRUE if yes
    bool isReadOnly();

    // to set the 'read only' state of the attribute
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReadOnly(bool y);

    // returns the default attribute value, may be an empty string
    const QCString & defaultValue();

    // to set the default attribute value ("" allowed)
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_DefaultValue(const char * s);

    // returns the attribute UML type
    const UmlTypeSpec & type();

    // to set the attribute UML type
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Type(const UmlTypeSpec & t);

    // returns the multiplicity (may be an empty string)
    const QCString & multiplicity();

    // to set the multiplicity
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Multiplicity(const char * s);

    // returns the 'get' operation of the attribute, or 0 if it does not exist
    UmlOperation * getOperation();

    // to generate an associated 'get' operation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool addGetOperation();

    // returns the 'set' operation of the attribute, or 0 if it does not exist
    
    UmlOperation * setOperation();

    // to generate an associated 'set' operation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool addSetOperation();

#ifdef WITHCPP
    // Indicate if the attribute is 'mutable'
    bool isCppMutable();

    // Set if the attribute is 'mutable'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isCppMutable(bool y);
#endif
#ifdef WITHJAVA
    // indicates if the attribute is 'transient', returns TRUE if yes
    bool isJavaTransient();

    // to set the 'transient' state of the attribute
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isJavaTransient(bool y);
#endif

#ifdef WITHIDL
    // in case the attribute is an IDL union's member returns the
    // corresponding 'case', an empty string in case it is not specified
    QCString idlCase();

    // to set the 'case' even the attribute is not (already) known as
    // an IDL union's member
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlCase(UmlAttribute * a);

    // to set the 'case' even the attribute is not (already) known as
    // an IDL union's member
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlCase(const char * s);
#endif

    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    bool _read_only : 1;

#ifdef WITHCPP
    bool _cpp_mutable : 1;
#endif
#ifdef WITHJAVA
    bool _java_transient : 1;
#endif

    QCString _multiplicity;

    QCString _default_value;

    UmlTypeSpec _type;

    UmlOperation * _get_oper;

    UmlOperation * _set_oper;

#ifdef WITHIDL
    // exclusive with idl_explicit_case
    UmlAttribute * _idl_case;

    QCString _idl_explicit_case;
#endif


  public:
    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseAttribute(void * id, const QCString & n);


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

#ifdef WITHPHP
    virtual void read_php_();
#endif

#ifdef WITHPYTHON
    virtual void read_python_();
#endif

#ifdef WITHIDL
    virtual void read_idl_();
#endif

};

inline UmlBaseAttribute::UmlBaseAttribute(void * id, const QCString & n) : UmlClassMember(id, n) {
  _get_oper = 0;
  _set_oper = 0;
  
#ifdef WITHIDL
  _idl_case = 0;
#endif
}

#endif
