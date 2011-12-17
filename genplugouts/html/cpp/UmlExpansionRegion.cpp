
#include "UmlExpansionRegion.h"

QCString UmlExpansionRegion::sKind() {
  return "expansion region";
}

void UmlExpansionRegion::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlItem::html(pfix, rank, "Expansion region", level, "expansionregion");

  unload(FALSE, FALSE);
}

