#ifndef _UMLARTIFACT_H
#define _UMLARTIFACT_H


#include "UmlBaseArtifact.h"
#include <qcstring.h>

class FileIn;
class Token;
class UmlItem;

// This class manages 'artifacts'
//
// You can modify it as you want (except the constructor)
class UmlArtifact : public UmlBaseArtifact {
  public:
    UmlArtifact(void * id, const QCString & n) : UmlBaseArtifact(id, n) { NumberOf += 1; };

    //  call at end of import : try to manifestation
    virtual void solveManifestation(QCString s, QCString idref);

    static void init();

    //import the artifact starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    static int numberOf() { return NumberOf; };


  protected:
    static int NumberOf;

};

#endif
