#ifndef _UMLNCRELATION_H
#define _UMLNCRELATION_H


#include "UmlBaseNcRelation.h"
#include <qcstring.h>

class FileOut;

//This class manages 'relations' between non class objects
//
// You can modify it as you want (except the constructor)
class UmlNcRelation : public UmlBaseNcRelation {
  public:
    UmlNcRelation(void * id, const QCString & n) : UmlBaseNcRelation(id, n) {};

    virtual bool write_if_needed(FileOut & out);

    virtual void write_generalization(FileOut & out);

    virtual void write_dependency(FileOut & out);

};

#endif
