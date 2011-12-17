#ifndef _UMLACTUALPARAMETER_H
#define _UMLACTUALPARAMETER_H


#include "UmlBaseActualParameter.h"

class FileOut;
class UmlClass;

// This class manages 'actual parameters' of a template class. For instance T in
//
//	class Cl1 : public Cl2<T> ...
//
// You can modify it as you want (except the constructor)
class UmlActualParameter : public UmlBaseActualParameter {
  public:
    UmlActualParameter() {};

    void write(FileOut & out, UmlClass * cl, int actualrank) const;

};

#endif
