
#include "UmlPackage.h"

void UmlPackage::uml2cpp(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2cpp(rec);
  }
}

void UmlPackage::uml2java(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2java(rec);
  }
}

void UmlPackage::uml2idl(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2idl(rec);
  }
}

void UmlPackage::uml2php(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2php(rec);
  }
}

void UmlPackage::uml2python(bool rec) {
  if (rec) {
    const QVector<UmlItem> ch = children();
    unsigned n = ch.size();
    
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->uml2python(rec);
  }
}

