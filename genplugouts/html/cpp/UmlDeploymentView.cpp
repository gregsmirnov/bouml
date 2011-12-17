
#include "UmlDeploymentView.h"

void UmlDeploymentView::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlItem::html(pfix, rank, "Deployment View", level, "view");

  unload(FALSE, FALSE);
}

QCString UmlDeploymentView::sKind() {
  return "deployment view";
}

bool UmlDeploymentView::chapterp() {
  return TRUE;
}

