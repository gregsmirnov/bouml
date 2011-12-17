
#include "UmlRegion.h"

QCString UmlRegion::sKind() {
  return "region";
}

void UmlRegion::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlItem::html(pfix, rank, "State region", level, "stateregion");

  unload(FALSE, FALSE);
}

bool UmlRegion::chapterp() {
  return TRUE;
}

