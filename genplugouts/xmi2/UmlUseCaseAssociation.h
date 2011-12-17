#ifndef _UMLUSECASEASSOCIATION_H
#define _UMLUSECASEASSOCIATION_H


#include "UmlBaseUseCaseAssociation.h"

class FileOut;
class UmlItem;

// this class manages association between use case and actor,
// you can modify it
class UmlUseCaseAssociation : public UmlBaseUseCaseAssociation {
  public:
    void write(FileOut & out, UmlItem * d, int rank, const char * k);

};

#endif
