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

    //called inside element definition
    
    virtual void write(FileOut & out);

    //inside is true when the generation is done inside the element definition
    virtual void write(FileOut & out, bool inside);

    //write definition inside the element
    virtual void write_generalization(FileOut & out);

    //write definition outside the element
    virtual void write_dependency(FileOut & out);

};

#endif
