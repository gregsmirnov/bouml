
#include "UmlClassView.h"

QCString UmlClassView::sKind() {
  return "class view";
}

void UmlClassView::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlItem::html(pfix, rank, "Class View", level, "view");

  unload(FALSE, FALSE);
}

bool UmlClassView::chapterp() {
  return TRUE;
}

