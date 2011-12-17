
#include "UmlItem.h"

#include "Dialog.h"
 UmlItem::~UmlItem() {
}

void UmlItem::projection() {
  BooL rec = (children().size() != 0);
  char lang;
  Dialog d(rec, lang);
  
  if (d.exec() == QDialog::Accepted) {
    switch (lang) {
    case 'c':
      uml2cpp(rec);
      break;
    case 'j':
      uml2java(rec);
      break;
    case 'i':
      uml2idl(rec);
      break;
    case 'h':
      uml2php(rec);
      break;
    case 'y':
      uml2python(rec);
      break;
    default:
      break;
    }
  }
}

void UmlItem::uml2cpp(bool) {
}

void UmlItem::uml2java(bool) {
}

void UmlItem::uml2idl(bool) {
}

void UmlItem::uml2php(bool) {
}

void UmlItem::uml2python(bool) {
}

