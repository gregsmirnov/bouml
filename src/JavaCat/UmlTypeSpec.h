#ifndef _UMLTYPESPEC_H
#define _UMLTYPESPEC_H


#include "UmlBaseTypeSpec.h"

// this class memorize a type specification, which may be
//
// - a class reference
//
// - an explicit type
class UmlTypeSpec : public UmlBaseTypeSpec {
#ifdef ROUNDTRIP
  public:
    bool equal(const UmlTypeSpec &) const;
#endif
};

#endif
