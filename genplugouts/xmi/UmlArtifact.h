#ifndef _UMLARTIFACT_H
#define _UMLARTIFACT_H


#include "UmlBaseArtifact.h"
#include <qcstring.h>

class FileOut;

// This class manages 'artifacts'
//
// You can modify it as you want (except the constructor)
class UmlArtifact : public UmlBaseArtifact {
  public:
    UmlArtifact(void * id, const QCString & n) : UmlBaseArtifact(id, n) {};

    virtual bool write_if_needed(FileOut & out);

};

#endif
