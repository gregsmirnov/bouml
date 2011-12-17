#ifndef _UMLUSECASE_H
#define _UMLUSECASE_H


#include "UmlBaseUseCase.h"
#include "Uc.h"
#include <qcstring.h>

class UmlItem;
class File;

// This class manages 'use case'
//
// You can modify it as you want (except the constructor)
class UmlUseCase : public UmlBaseUseCase, public Uc {
  public:
    UmlUseCase(void * id, const QCString & n) : UmlBaseUseCase(id, n) {
      cpt[kind()] += 1;
    };

    virtual UmlItem * item();

    virtual void readObject(File & f, QCString s);

    static void import(File & f, UmlItem * parent);

    //Just to not have warning
    virtual ~UmlUseCase();

};

#endif
