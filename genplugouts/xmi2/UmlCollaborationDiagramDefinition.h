#ifndef _UMLCOLLABORATIONDIAGRAMDEFINITION_H
#define _UMLCOLLABORATIONDIAGRAMDEFINITION_H


#include "UmlBaseCollaborationDiagramDefinition.h"

class FileOut;
class UmlItem;

// this class manages collaboration diagram definition,
// you can modify it
class UmlCollaborationDiagramDefinition : public UmlBaseCollaborationDiagramDefinition {
  public:
    virtual ~UmlCollaborationDiagramDefinition();

    virtual void write(FileOut & out, UmlItem * diag);

};

#endif
