
#include "UmlBaseRegion.h"
#include "UmlRegion.h"
#include "UmlState.h"

UmlRegion * UmlBaseRegion::create(UmlState * parent, const char * s)
{
  return (UmlRegion *) parent->create_(aRegion, s);
}

anItemKind UmlBaseRegion::kind() {
  return aRegion;
}

