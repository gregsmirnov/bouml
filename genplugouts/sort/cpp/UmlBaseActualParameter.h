#ifndef _UMLBASEACTUALPARAMETER_H
#define _UMLBASEACTUALPARAMETER_H


#include "UmlTypeSpec.h"

class UmlClass;

// This class manages 'actual parameters' of a template class. For instance T in
//
//	class Cl1 : public Cl2<T> ...
class UmlBaseActualParameter {
  public:
    UmlBaseActualParameter() {};

    // returns the class of the corresponding formal
    UmlClass * superClass() const;

    // returns the actual rank (0...)
    unsigned rank() const;

    // returns the actual's value
    const UmlTypeSpec & value() const;

  friend class UmlBaseClass;

  protected:
    UmlClass * _superClass;

    unsigned _rank;

    UmlTypeSpec _value;

    //internal, do NOT use it
    
    void read_();

};

#endif
