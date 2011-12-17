
#include "UmlBaseInterruptibleActivityRegion.h"
#include "UmlInterruptibleActivityRegion.h"
#include "UmlItem.h"

UmlInterruptibleActivityRegion * UmlBaseInterruptibleActivityRegion::create(UmlItem * parent, const char * s)
{
  return (UmlInterruptibleActivityRegion *) parent->create_(anInterruptibleActivityRegion, s);
}

anItemKind UmlBaseInterruptibleActivityRegion::kind() {
  return anInterruptibleActivityRegion;
}

