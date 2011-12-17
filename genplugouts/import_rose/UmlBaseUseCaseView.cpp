
#include "UmlBaseUseCaseView.h"
#include "UmlUseCaseView.h"

UmlUseCaseView * UmlBaseUseCaseView::create(UmlItem * parent, const char * s)
{
  return (UmlUseCaseView *) parent->create_(anUseCaseView, s);
}

anItemKind UmlBaseUseCaseView::kind() {
  return anUseCaseView;
}

