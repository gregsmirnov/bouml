#ifndef _UMLBASETYPESPEC_H
#define _UMLBASETYPESPEC_H


#include <qcstring.h>

class UmlClass;

// this class memorize a type specification, which may be
//
// - a class reference
//
// - an explicit type
class UmlBaseTypeSpec {
  public:
    UmlBaseTypeSpec() : type(0) {};

    // returns the type as a string in all cases
    const QCString & toString() const;

    // significant in case type == 0
    QCString explicit_type;

    UmlClass * type;

};

#endif
