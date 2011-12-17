
#include "UmlComponentView.h"

QCString UmlComponentView::sKind() {
  return "component view";
}

void UmlComponentView::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlItem::html(pfix, rank, "Component View", level, "view");

  unload(FALSE, FALSE);
}

bool UmlComponentView::chapterp() {
  return TRUE;
}

