#ifndef _UMLOPERATION_H
#define _UMLOPERATION_H


#include "UmlBaseOperation.h"
#include <qcstring.h>
#include "aDirection.h"

class UmlClass;

// This class manages 'operations', notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlOperation : public UmlBaseOperation {
  public:
    UmlOperation(void * id, const QCString & n) : UmlBaseOperation(id, n), _managed(FALSE) {};

    //  set the default C++/Java definition
    //  set it usefull
    
    void defaultDef();

    //  set the return type in C++/Java
    //
    
    void setType(const char * type, const char * s);

    void setType(UmlClass * type, const char * s);

    //  add a parameter
    void addParam(int rank, aDirection dir, const char * name, UmlClass * type);

    //  set the params between ( and ${]} in C++/Java
    
    void setParams(const char * s);

    void managed();

    bool isManaged() const;

    //  mark the element  is usefull
    virtual void setUsed();


  protected:
    bool _managed;

};

inline void UmlOperation::managed() {
  _managed = TRUE;
}

inline bool UmlOperation::isManaged() const {
  return _managed;
}

#endif
