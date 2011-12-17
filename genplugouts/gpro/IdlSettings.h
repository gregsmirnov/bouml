#ifndef _IDLSETTINGS_H
#define _IDLSETTINGS_H

#ifdef WITHIDL


#include <qcstring.h>
#include "UmlSettings.h"
#include <qdict.h>

// This class manages settings concerning IDL, configured through
// the 'Generation settings' dialog.
//
// This class may be defined as a 'singleton', but I prefer to use static 
// members allowing to just write 'IdlSettings::member' rather than
// 'IdlSettings::instance()->member' or other long sentence like this.
class IdlSettings : public UmlSettings {
  public:
    // returns TRUE when the created Java objects are initialized
    // with the default declaration/definition
    static bool useDefaults();

    // if y is TRUE the future created Java objects will be initialized
    // with the default declaration/definition
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    
    static bool set_UseDefaults(bool y);

    // returns the IDL type corresponding to the 'UML' type given in
    // argument, as it is configured in the first 'Generation settings'
    // dialog's tab
    static QCString type(QCString s);

    // set the IDL type corresponding to the 'UML' type given in
    // argument, as it is configured in the first 'Generation settings'
    // dialog's tab
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_Type(QCString s, QCString v);

    // reverse of the Type() operation, returns the 'UML' type corresponding
    // to the IDL type given in argument
    static QCString umlType(QCString s);

    // returns the IDL stereotype corresponding to the 'UML' stereotype given
    // in argument
    static QCString relationStereotype(const QCString & s);

    // set the IDL stereotype corresponding to the 'UML' stereotype given
    // in argument
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RelationStereotype(QCString s, QCString v);

    // reverse of the RelationStereotype() operation, returns the 'UML' 
    // stereotype corresponding to the IDL one given in argument
    static QCString relationUmlStereotype(QCString s);

    // returns the IDL stereotype corresponding to the 'UML' stereotype given
    // in argument
    static QCString classStereotype(QCString s);

    // set the IDL stereotype corresponding to the 'UML' stereotype given
    // in argument
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ClassStereotype(QCString s, QCString v);

    // reverse of the ClassStereotype() operation, returns the 'UML' 
    // stereotype corresponding to the IDL one given in argument
    static QCString classUmlStereotype(QCString s);

    // returns the #include or other form specified in the last 
    // 'Generation settings' tab for the Idl type given in argument.
    static QCString include(QCString s);

    // set the #include or other form specified in the last 
    // 'Generation settings' tab for the Idl type given in argument.
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_Include(QCString s, QCString v);

    // returns the 'root' directory 
    static const QCString & rootDir();

    // set the 'root' directory 
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RootDir(QCString v);

    // returns the default source file content
    static const QCString & sourceContent();

    // set the default source file content
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SourceContent(QCString v);

    // returns the extension of the files produced by the Idl code generator
    static const QCString & sourceExtension();

    // set the extension of the files produced by the Idl code generator
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SourceExtension(QCString v);

    // returns the default definition of an interface
    static const QCString & interfaceDecl();

    // set the default definition of an interface
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_InterfaceDecl(QCString v);

    // returns the default definition of a valuetype
    static const QCString & valuetypeDecl();

    // set the default definition of a valuetype
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ValuetypeDecl(QCString v);

    // returns the default specification for an 'external' class
    static const QCString & externalClassDecl();

    // set the default specification for an 'external' class
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ExternalClassDecl(QCString v);

    // returns the default definition of a struct
    static const QCString & structDecl();

    // set the default definition of a struct
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_StructDecl(QCString v);

    // returns the default definition of a typedef
    static const QCString & typedefDecl();

    // set the default definition of a typedef
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_TypedefDecl(QCString v);

    // returns the default definition of an exception
    static const QCString & exceptionDecl();

    // set the default definition of an exception
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ExceptionDecl(QCString v);

    // returns the default definition of an union
    static const QCString & unionDecl();

    // set the default definition of an union
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_UnionDecl(QCString v);

    // returns the default definition of an enum
    static const QCString & enumDecl();

    // set the default definition of an enum
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_EnumDecl(QCString v);

    // returns the default definition of an attribute
    static const QCString & attributeDecl();

    // set the default definition of an attribute
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_AttributeDecl(QCString v);

    // returns the default definition of an attribute
    // placed in a valuetype
    static const QCString & valuetypeAttributeDecl();

    // set the default definition of an attribute
    // placed in a valuetype
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ValuetypeAttributeDecl(QCString v);

    // returns the default definition of an union item
    static const QCString & unionItemDecl();

    // set the default definition of an union item
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_UnionItemDecl(QCString v);

    // returns the default definition of an enumeration item
    static const QCString & enumItemDecl();

    // set the default definition of an enumeration item
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_EnumItemDecl(QCString v);

    // returns the default definition of a constant attribute
    static const QCString & constDecl();

    // set the default definition of a constant attribute
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ConstDecl(QCString v);

    // returns the default definition of a relation depending on the
    // multiplicity given in argument.
    static const QCString & relationDecl(const char * multiplicity);

    // set the default definition of a relation depending on the
    // multiplicity given in argument.
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RelationDecl(const char * multiplicity, QCString v);

    // returns the default definition of a relation in a valuetype
    // depending on the multiplicity given in argument.
    static const QCString & valuetypeRelationDecl(const char * multiplicity);

    // set the default definition of a relation in a valuetype
    // depending on the multiplicity given in argument.
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ValuetypeRelationDecl(const char * multiplicity, QCString v);

    // returns the default definition of a relation in an union
    // depending on the multiplicity given in argument.
    static const QCString & unionRelationDecl(const char * multiplicity);

    // set the default definition of a relation in an union
    // depending on the multiplicity given in argument.
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_UnionRelationDecl(const char * multiplicity, QCString v);

    // returns the default declaration of an operation
    static const QCString & operationDecl();

    // set the default declaration of an operation
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_OperationDecl(QCString v);

    // returns the default name of a 'get' operation generated through
    // the attribute and relation 'add get operation' menu
    static const QCString & getName();

    // set the default name of a 'get' operation generated through
    // the attribute and relation 'add get operation' menu
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_GetName(QCString v);

    // returns the default name of a 'set' operation generated 
    // through the attribute and relation 'add set operation' menu
    static const QCString & setName();

    // set the default name of a 'set' operation generated 
    // through the attribute and relation 'add set operation' menu
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SetName(QCString v);

    // returns if a 'set' operation generated through the attribute
    // and relation 'add set operation' menu is oneway by default
    static bool isSetOneway();

    // set if a 'set' operation generated through the attribute
    // and relation 'add set operation' menu is oneway by default
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_IsSetOneway(bool v);


  private:
    // never called !
    IdlSettings() {};

    static bool _defined;

    static QCString _root;

    static QCString _interface_decl;

    static QCString _valuetype_decl;

    static QCString _struct_decl;

    static QCString _typedef_decl;

    static QCString _exception_decl;

    static QCString _union_decl;

    static QCString _enum_decl;

    static QCString _external_class_decl;

    static QCString _attr_decl;

    static QCString _valuetype_attr_decl;

    static QCString _union_item_decl;

    static QCString _enum_item_decl;

    static QCString _const_decl;

    static QCString _rel_decl[3/*multiplicity*/];

    static QCString _valuetype_rel_decl[3/*multiplicity*/];

    static QCString _union_rel_decl[3/*multiplicity*/];

    static QCString _oper_decl;

    static QCString _get_name;

    static QCString _set_name;

    static bool _is_set_oneway;

    static QCString _src_content;

    static QCString _ext;

    static QDict<QCString> _map_includes;


  protected:
    static void read_();

    static void read_if_needed_();

};


#endif

#endif
