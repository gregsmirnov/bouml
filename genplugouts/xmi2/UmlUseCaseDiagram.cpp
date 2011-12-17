
#include "UmlUseCaseDiagram.h"
#include "FileOut.h"

#include "UmlUseCaseDiagramDefinition.h"
void UmlUseCaseDiagram::write(FileOut &) {
  parent()->memo_ac_uc_assoc(this);
}

void UmlUseCaseDiagram::write_it(FileOut & out) {
  const char * k = ((_uml_20) ? "ownedMember" : "packagedElement");
  UmlUseCaseDiagramDefinition * def = definition();
  const QVector<UmlUseCaseAssociation> & assocs = def->associations();
  int n = (int) assocs.size();
  
  for (int rank = 0; rank != n; rank += 1)
    assocs.at(rank)->write(out, this, rank, k);
 
  unload(); 
}

