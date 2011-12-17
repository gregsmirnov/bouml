#ifndef _UMLSETTINGS_H
#define _UMLSETTINGS_H


#include <qcstring.h>
#include "aLanguage.h"
#include <qdict.h>

struct UmlBuiltin;
struct UmlStereotype;

// This class manages settings not linked with a language, configured through
// the 'Generation settings' dialog.
//
// This class may be defined as a 'singleton', but I prefer to use static 
// members allowing to just write 'UmlSettings::member' rather than
// 'UmlSettings::instance()->member' or other long sentence like this.

class UmlSettings {
  public:
    // return the default description
    static QCString artifactDescription();

    // set the default description
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ArtifactDescription(QCString v);

    // return the default description
    static QCString classDescription();

    // set the default description
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_ClassDescription(QCString v);

    // return the default description
    static QCString operationDescription();

    // set the default description
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_OperationDescription(QCString v);

    // return the default description
    static QCString attributeDescription();

    // set the default description
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_AttributeDescription(QCString v);

    // return the default description
    static QCString relationDescription();

    // set the default description
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_RelationDescription(QCString v);

    // return the language from which the getter's name rule must be followed at Uml level
    static aLanguage umlGetName();

    // set the language from which the getter's name rule must be followed at Uml level
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_UmlGetName(aLanguage v);

    // return the language from which the setter's name rule must be followed at Uml level
    static aLanguage umlSetName();

    // set the language from which the setter's name rule must be followed at Uml level
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    static bool set_UmlSetName(aLanguage v);


  protected:
    // never called !
    UmlSettings() {};

    static bool _defined;


  private:
    static aLanguage _uml_get_name;

    static aLanguage _uml_set_name;


  protected:
    static QCString _artifact_default_description;

    static QCString _class_default_description;

    static QCString _operation_default_description;

    static QCString _attribute_default_description;

    static QCString _relation_default_description;

    static QDict<UmlBuiltin> _map_builtins;

    static UmlBuiltin * _builtins;

    static QDict<UmlStereotype> _map_relation_attribute_stereotypes;

    static UmlStereotype * _relation_attribute_stereotypes;

    static QDict<UmlStereotype> _map_class_stereotypes;

    static UmlStereotype * _class_stereotypes;

    static void read_();

    static void read_if_needed_();

    static unsigned multiplicity_column(const QCString & mult);

    static QCString uml_type(const QCString & t, QCString UmlBuiltin::* f);

    static QCString uml_rel_attr_stereotype(const QCString & t, QCString UmlStereotype::* f);

    static QCString uml_class_stereotype(const QCString & t, QCString UmlStereotype::* f);

    static UmlBuiltin * add_type(const QCString & s);

    static UmlStereotype * add_rel_attr_stereotype(const QCString & s);

    static UmlStereotype * add_class_stereotype(const QCString & s);

};

#endif
