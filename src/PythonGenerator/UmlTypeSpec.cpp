
#include <qtextstream.h> 

#include "UmlTypeSpec.h"
#include "UmlClass.h"

void UmlTypeSpec::generate_import(QTextOStream & f, UmlArtifact * using_art, 
				  bool from, QCString & made) const {
  if (type != 0)
    type->generate_import(f, using_art, from, made);
}


