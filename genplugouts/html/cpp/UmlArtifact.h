#ifndef _UMLARTIFACT_H
#define _UMLARTIFACT_H


#include "UmlBaseArtifact.h"
#include <qcstring.h>

// This class manages 'artifacts'
//
// You can modify it as you want (except the constructor)
class UmlArtifact : public UmlBaseArtifact {
  public:
    UmlArtifact(void * id, const QCString & n) : UmlBaseArtifact(id, n) {};

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(QCString pfix, unsigned int rank, unsigned int level);

    //returns a string indicating the king of the element
    virtual QCString sKind();

};

#endif
