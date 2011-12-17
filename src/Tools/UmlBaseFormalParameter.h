#ifndef _UMLBASEFORMALPARAMETER_H
#define _UMLBASEFORMALPARAMETER_H


#include <qcstring.h>
#include "UmlTypeSpec.h"

// This class manages 'formal parameters' of a template class. For instance 'T' in 
//
//	template class Cl<class T> ...
class UmlBaseFormalParameter {
  public:
    UmlBaseFormalParameter() {};

    // returns the name of the formal
    const QCString & name() const { return _name; };

    // returns the type of a formal, probably "class"
    const QCString & type() const { return _type; };

    // returns the default actual value
    const UmlTypeSpec & defaultValue() const { return _default_value; };

    //  return the optional extend (Java)
    const UmlTypeSpec & extend() const { return _extends; };
    friend class UmlBaseClass;

  protected:
    QCString _name;

    // "class" ...
    QCString _type;

    UmlTypeSpec _default_value;

    UmlTypeSpec _extends;

    //internal, do NOT use it
    
    void read_();

};

#endif
