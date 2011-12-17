#ifndef _UMLBASENCRELATION_H
#define _UMLBASENCRELATION_H


#include "UmlItem.h"
#include "aRelationKind.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlNcRelation;

// Manage the relations between non class objects, for instances
// dependency between packages and use case generalization
class UmlBaseNcRelation : public UmlItem {
  public:
    // returns a new non class relation of the given 'kind' from 'start' to 'end'
    //
    // In case it cannot be created ('parent' cannot contain it etc ...)
    // return 0 in C++ and produce a RuntimeException in Java
    static UmlNcRelation * create(aRelationKind kind, UmlItem * start, UmlItem * end);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the kind of the relation : aGeneralisation or a Dependency, the other are not allowed
    // for the non class relations
    aRelationKind relationKind();

    // returns the 'end' object (the 'start' object is the parent of the relation) no set !
    UmlItem * target();


  private:
    aRelationKind _rel_kind;

    UmlItem * _target;


  protected:
    //internal, do NOT use it
    
    virtual void read_uml_();

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseNcRelation(void * id, const QCString & n);

};

inline UmlBaseNcRelation::UmlBaseNcRelation(void * id, const QCString & n) : UmlItem(id, n) {
  _target = 0;
}

#endif
