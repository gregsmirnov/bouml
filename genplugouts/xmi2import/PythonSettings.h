#ifndef _PYTHONSETTINGS_H
#define _PYTHONSETTINGS_H

#ifdef WITHPYTHON


#include <qcstring.h>
#include "UmlSettings.h"
#include <qdict.h>

// This class manages settings concerning PYTHON, configured through
// the 'Generation settings' dialog.
//
// This class may be defined as a 'singleton', but I prefer to use static
// members allowing to just write 'PythonSettings::member' rather than
// 'PythonSettings::instance()->member' or other long sentence like this.
class PythonSettings : public UmlSettings {
  public:
    // return if classes follow Python 2.2 by default
    static bool isPython_2_2();

    // set if classes follow Python 2.2 by default
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_IsPython_2_2(bool y);

    // returns if operations follow Python 3 (pep-3107)
    static bool isPython_3_operation();

    // set if operations follow Python 3 (pep-3107)
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_IsPython_3_operation(bool y);

    // return the  default indent step
    static const QCString & indentStep();

    // set default indent step
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_IndentStep(char * v);

    // returns TRUE when the created Python objects are initialized
    // with the default declaration/definition
    static bool useDefaults();

    // if y is TRUE the future created Python objects will be initialized
    // with the default declaration/definition
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_UseDefaults(bool y);

    // returns the PYTHON stereotype corresponding to the 'UML' stereotype given
    // in argument
    static QCString relationAttributeStereotype(const QCString & s);

    // set the PYTHON stereotype corresponding to the 'UML' stereotype given
    // in argument
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RelationAttributeStereotype(QCString s, QCString v);

    // reverse of the RelationAttributeStereotype() operation, returns the 'UML' 
    // stereotype corresponding to the PYTHON one given in argument
    static QCString relationAttributeUmlStereotype(const QCString & s);

    // returns the PYTHON stereotype corresponding to the 'UML' stereotype given
    // in argument
    static QCString classStereotype(const QCString & s);

    // set the PYTHON stereotype corresponding to the 'UML' stereotype given
    // in argument
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ClassStereotype(QCString s, QCString v);

    // reverse of the ClassStereotype() operation, returns the 'UML' 
    // stereotype corresponding to the PYTHON one given in argument
    static QCString classUmlStereotype(const QCString & s);

    //returns the import or other form specified in the last
    //'Generation settings' tab for the Python type given in argument.
    static QCString get_import(const QCString & s);

    //  set the import or other form specified in the last
    //  'Generation settings' tab for the Python type given in argument.
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_Import(QCString s, QCString v);

    //  return the 'root' directory
    static const QCString & rootDir();

    //  set the 'root' directory
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RootDir(QCString v);

    // returns the default source file content
    static const QCString & sourceContent();

    // set the default source file content
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SourceContent(QCString v);

    // returns the extension of the file produced by the PYTHON code generator
    static const QCString & sourceExtension();

    // set the extension of the file produced by the PYTHON code generator
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_SourceExtension(QCString v);

    // returns the default definition of a class
    static const QCString & classDecl();

    // set the default definition of a class
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ClassDecl(QCString v);

    // returns the default specification for an 'external' class
    static const QCString & externalClassDecl();

    // set the default specification for an 'external' class
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ExternalClassDecl(QCString v);

    // returns the default definition of an enumeration
    static QCString enumDecl();

    // set the default definition of an enumeration
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_EnumDecl(QCString v);

    // returns the default definition of an attribute depending on the multiplicity
    static const QCString & attributeDecl(const char * multiplicity);

    // set the default definition of an attribute
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_AttributeDecl(const char * multiplicity, QCString v);

    // returns the default definition of an enumeration item
    static QCString enumItemDecl();

    // set the default definition of an enumeration item
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_EnumItemDecl(QCString v);

    // returns the default definition of a relation depending on it is an
    // aggregation by value or not and the multiplicity, given in argument.
    static const QCString & relationDecl(bool by_value, const char * multiplicity);

    // set the default definition of a relation depending on it is an
    // aggregation by value or not and the multiplicity, given in argument.
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RelationDecl(bool by_value, const char * multiplicity, const char * v);

    // returns the default definition of an operation
    static const QCString & operationDef();

    // set the default definition of an operation
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_OperationDef(QCString v);

    // returns the default definition of __init__
    static const QCString & initOperationDef();

    // set the default definition of __init__
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_InitOperationDef(QCString v);

    // returns the default name of a 'get' operation generated 
    // through the attribute and relation 'add get operation' menu
    static const QCString & getName();

    // set the default name of a 'get' operation generated 
    // through the attribute and relation 'add get operation' menu
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


  private:
    // never called !
    PythonSettings() {};

    static unsigned mult_column(const QCString & mult);

    static bool _defined;

    static bool _2_2;


  protected:
    static bool _operation_3;


  private:
    static QCString _indent_step;

    static QCString _root;

    static QCString _class_decl;

    static QCString _external_class_decl;

    static QCString _enum_decl;

    static QCString _attr_decl[2/*multiplicity*/];

    static QCString _enum_item_decl;

    static QCString _rel_decl[2/*relation kind*/][2/*multiplicity*/];

    static QCString _oper_def;


  protected:
    static QCString _initoper_def;


  private:
    static QCString _get_name;

    static QCString _set_name;

    static QCString _src_content;

    static QCString _ext;

    static QDict<QCString> _map_imports;


  protected:
    static void read_();

    static void read_if_needed_();

};


#endif

#endif
