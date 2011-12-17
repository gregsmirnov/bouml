#ifndef _UMLTYPESPEC_H
#define _UMLTYPESPEC_H


#include "UmlBaseTypeSpec.h"

class QTextOStream;
class UmlArtifact;

// this class memorize a type specification, which may be
//
// - a class reference
//
// - an explicit type
class UmlTypeSpec : public UmlBaseTypeSpec {
  public:
    void generate_require_onces(QTextOStream & f,
				QCString & made,
				UmlArtifact * using_art) const;
};

#endif
