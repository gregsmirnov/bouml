#ifndef _UMLCLASS_H
#define _UMLCLASS_H


#include "UmlBaseClass.h"
#include <qcstring.h>
#include "Language.h"
#include <qdict.h>

class File;
class UmlItem;
class UmlTypeSpec;
class Artifact;

// This class manages 'classes', notes that the class 'ClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)
class UmlClass : public UmlBaseClass {
  public:
    UmlClass(void * id, const QCString & n) : UmlBaseClass(id, n) {
      cpt[kind()] += 1;
    };

    Language language() const;

    static UmlClass * import(File & f, UmlItem * parent, const QCString & knd);

    static bool replaceType(UmlTypeSpec & t, QCString & target_id, const QCString & ts);


  private:
    void importAttributes(File & f);

    void importOperations(File & f);

    void importRelations(File & f);

    void importInstantiate(File & f);

    void importActuals(File & f);

    void importFormals(File & f);

    void importClasses(File & f);

    static void importIdlConstant(UmlItem * parent, const QCString & id, const QCString & s, const QCString & doc, QDict<QCString> & prop);

    void cplusplus(QDict<QCString> & prop);

    void oracle8(QDict<QCString> & prop);

    void corba(QDict<QCString> & prop);

    void java(QDict<QCString> & prop);

    void assocArtifact(Artifact * c, QCString & art_path);


  public:
    const QCString file() const;

    const QCString bodyFile() const;


  protected:
    Language lang;

    QCString _file;

    QCString _body_file;

};

inline Language UmlClass::language() const {
  return lang;
}

inline const QCString UmlClass::file() const {
  return _file;
}

inline const QCString UmlClass::bodyFile() const {
  return _body_file;
}

#endif
