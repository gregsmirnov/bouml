#ifndef _UMLTYPESPEC_H
#define _UMLTYPESPEC_H


#include "UmlBaseTypeSpec.h"

// this class memorize a type specification, which may be
//
// - a class reference
//
// - an explicit type
class UmlTypeSpec : public UmlBaseTypeSpec {
  public:
    UmlTypeSpec() {}
    UmlTypeSpec(const char * t) { explicit_type = t; }
    UmlTypeSpec(UmlClass * t) { type = t; }
};

#endif
