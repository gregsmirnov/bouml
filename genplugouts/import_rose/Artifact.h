#ifndef _ARTIFACT_H
#define _ARTIFACT_H


#include "Language.h"
#include <qcstring.h>
#include <qasciidict.h>

class File;
class UmlPackage;
class UmlClass;

class Artifact {
  protected:
    Language language;

    QCString name;

    QCString stereotype;

    QCString description;

    QCString cpp_path;

    QCString java_path;

    QCString cpp_namespace;

    QCString java_package;

    QCString idl_module;

    static QAsciiDict<Artifact> all;


  public:
    static Artifact * find(const QCString & uid);

    static void import_component_view(File & f);

    static void import_components(File & f);

    static void import(File & f);

    //'pack' is the package containing the class view where 'cl' is defined
    //'cl' is not nested in an other class
    //'art_path' is the rose path of the artifact
    void add(UmlPackage * pack, UmlClass * cl, QCString & art_path);

    void add_cpp(UmlPackage * pack, UmlClass * cl, QCString h, QCString src);

    void add_corba(UmlPackage * pack, UmlClass * cl, QCString src);

    void add_java(UmlPackage * pack, UmlClass * cl, QCString art_path, QCString src);

    QCString normalize(QCString path, QCString root);

};

#endif
