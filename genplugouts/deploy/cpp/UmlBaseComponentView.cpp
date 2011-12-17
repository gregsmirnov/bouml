
#include "UmlBaseComponentView.h"
#include "UmlComponentView.h"
#include "UmlPackage.h"

UmlComponentView * UmlBaseComponentView::create(UmlPackage * parent, const char * s)
{
  return (UmlComponentView *) parent->create_(aComponentView, s);
}

anItemKind UmlBaseComponentView::kind() {
  return aComponentView;
}

