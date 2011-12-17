#ifndef _UMLATTRIBUTE_H
#define _UMLATTRIBUTE_H


#include "UmlBaseAttribute.h"
#include <qcstring.h>
#include <qdict.h>

class File;
class UmlClass;

// This class manages 'attribute', notes that the class 'UmlClassItem'
// is a mother class of the class's children.
//
// You can modify it as you want (except the constructor)
class UmlAttribute : public UmlBaseAttribute {
  public:
    UmlAttribute(void * id, const QCString & n) :  UmlBaseAttribute(id, n) {
      cpt[kind()] += 1;
    };


  private:
    void import(File & f);


  public:
    static void import(File & f, UmlClass * parent);

    static void importIdlConstant(UmlClass * parent, const QCString & id, const QCString & s, const QCString & doc, QDict<QCString> & prop);


  private:
    void cplusplus(QDict<QCString> & prop);

    void oracle8(QDict<QCString> & prop);

    void corba(QDict<QCString> & prop);

    void java(QDict<QCString> & prop);

};

#endif
