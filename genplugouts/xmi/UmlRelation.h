#ifndef _UMLRELATION_H
#define _UMLRELATION_H


#include "UmlBaseRelation.h"
#include <qcstring.h>
#include "aRelationKind.h"

class FileOut;

// This class manages 'relations' between classes, notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlRelation : public UmlBaseRelation {
  public:
    UmlRelation(void * id, const QCString & n) : UmlBaseRelation(id, n) {};

    virtual bool write_if_needed(FileOut & out);

    virtual void write_generalization(FileOut & out);

    virtual void write_dependency(FileOut & out);

    virtual void write_relation(FileOut & out);

    virtual void write_relation(FileOut & out, aRelationKind k);

};

#endif
