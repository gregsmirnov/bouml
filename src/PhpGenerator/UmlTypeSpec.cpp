#include <qtextstream.h> 

#include "UmlTypeSpec.h"
#include "UmlClass.h"

void UmlTypeSpec::generate_require_onces(QTextOStream & f,
					 QCString & made,
					 UmlArtifact * using_art) const {
  if (type != 0)
    type->generate_require_onces(f, made, using_art);
}
