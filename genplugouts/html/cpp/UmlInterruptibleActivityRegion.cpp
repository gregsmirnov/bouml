
#include "UmlInterruptibleActivityRegion.h"

QCString UmlInterruptibleActivityRegion::sKind() {
  return "interruptible activity region";
}

void UmlInterruptibleActivityRegion::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlItem::html(pfix, rank, "Interruptible activity region", level, "interruptibleactivityregion");

  unload(FALSE, FALSE);
}

