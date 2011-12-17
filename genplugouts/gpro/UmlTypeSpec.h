#ifndef _UMLTYPESPEC_H
#define _UMLTYPESPEC_H


#include <qcstring.h>

class UmlClass;

// this class memorize a type specification, which may be
//
// - a class reference
//
// - an explicit type
class UmlTypeSpec {
  public:
    // significant in case type == 0
    QCString explicit_type;

    // if != 0 the class representing the type
    UmlClass * type;

    UmlTypeSpec() : type(0) {};

    // returns the type as a string in all cases
    const QCString & toString() const;

};

#endif
