#ifndef _UMLUSECASE_H
#define _UMLUSECASE_H


#include "UmlBaseUseCase.h"
#include <qcstring.h>

// This class manages 'use case'
//
// You can modify it as you want (except the constructor)
class UmlUseCase : public UmlBaseUseCase {
  public:
    virtual void sort();

    UmlUseCase(void * id, const QCString & n) : UmlBaseUseCase(id, n) {};

    virtual int orderWeight();

};

#endif
