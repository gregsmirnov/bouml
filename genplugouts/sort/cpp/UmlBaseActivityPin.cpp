
#include "UmlBaseActivityPin.h"
#include "UmlActivityPin.h"
#include "UmlActivityAction.h"

UmlActivityPin * UmlBaseActivityPin::create(UmlActivityAction * parent, const char * s)
{
    return (UmlActivityPin *) parent->create_(anActivityPin, s);
}

anItemKind UmlBaseActivityPin::kind() {
    return anActivityPin;
}

