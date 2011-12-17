#ifndef _UMLTYPESPEC_H
#define _UMLTYPESPEC_H

class QTextOStream;
class UmlArtifact;

#include "UmlBaseTypeSpec.h"

// this class memorize a type specification, which may be
//
// - a class reference
//
// - an explicit type
class UmlTypeSpec : public UmlBaseTypeSpec {
  public:
    void generate_import(QTextOStream & f, UmlArtifact * using_art, bool from, QCString & made) const;
};

#endif
