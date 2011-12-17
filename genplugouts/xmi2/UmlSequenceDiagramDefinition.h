#ifndef _UMLSEQUENCEDIAGRAMDEFINITION_H
#define _UMLSEQUENCEDIAGRAMDEFINITION_H


#include "UmlBaseSequenceDiagramDefinition.h"

class FileOut;
class UmlItem;

// this class manages sequence diagram definition,
// you can modify it
class UmlSequenceDiagramDefinition : public UmlBaseSequenceDiagramDefinition {
  public:
    virtual ~UmlSequenceDiagramDefinition();

    virtual void write(FileOut & out, UmlItem * diag);

};

#endif
