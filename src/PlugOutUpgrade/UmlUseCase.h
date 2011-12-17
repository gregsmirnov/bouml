#ifndef UMLUSECASE_H
#define UMLUSECASE_H

#include "UmlBaseUseCase.h"

// This class manages 'use case'
// You can modify it as you want (except the constructor)

class UmlUseCase : public UmlBaseUseCase {
  public:
    UmlUseCase(void * id, const QCString & n)
      : UmlBaseUseCase(id, n) {};
  
};

#endif
