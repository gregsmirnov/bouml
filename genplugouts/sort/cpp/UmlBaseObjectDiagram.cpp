
#include "UmlBaseObjectDiagram.h"
#include "UmlObjectDiagram.h"
#include "UmlItem.h"

UmlObjectDiagram * UmlBaseObjectDiagram::create(UmlItem * parent, const char * s)
{
    return (UmlObjectDiagram *) parent->create_(anObjectDiagram, s);
}

anItemKind UmlBaseObjectDiagram::kind() {
    return anObjectDiagram;
}

