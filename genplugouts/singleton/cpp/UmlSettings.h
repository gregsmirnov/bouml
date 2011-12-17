#ifndef _UMLSETTINGS_H
#define _UMLSETTINGS_H


#include <qdict.h>
#include <qcstring.h>

struct UmlBuiltin;
struct UmlStereotype;

//  Do NOT use it, refer to CppSettings, JavaSettings and IdlSettings classes

class UmlSettings {
  protected:
    // never called !
    UmlSettings() {};

    static bool _defined;

    static QDict<UmlBuiltin> _map_builtins;

    static UmlBuiltin * _builtins;

    static QDict<UmlStereotype> _map_relation_stereotypes;

    static UmlStereotype * _relation_stereotypes;

    static QDict<UmlStereotype> _map_class_stereotypes;

    static UmlStereotype * _class_stereotypes;

    static void read_();

    static void read_if_needed_();

    static unsigned multiplicity_column(const QCString & mult);

    static QCString uml_type(const QCString & t, QCString UmlBuiltin::* f);

    static QCString uml_rel_stereotype(const QCString & t, QCString UmlStereotype::* f);

    static QCString uml_class_stereotype(const QCString & t, QCString UmlStereotype::* f);

    static UmlBuiltin * add_type(const QCString & s);

    static UmlStereotype * add_rel_stereotype(const QCString & s);

    static UmlStereotype * add_class_stereotype(const QCString & s);

};

#endif
