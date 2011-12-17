#ifndef _UMLUSECASE_H
#define _UMLUSECASE_H


#include "UmlBaseUseCase.h"
#include <qcstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

// This class manages 'use case'
//
// You can modify it as you want (except the constructor)
class UmlUseCase : public UmlBaseUseCase {
  public:
    UmlUseCase(void * id, const QCString & n) : UmlBaseUseCase(id, n) { NumberOf += 1; };

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    //  call at end of import : try to solve extend or include dependency
    virtual void solve(int context, QCString idref);

    static void init();

    //import the use case starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    static void importExtendInclude(FileIn & in, Token & token, UmlItem * where);

    static void importExtensionPoint(FileIn & in, Token & token, UmlItem * where);

    static int numberOf() { return NumberOf; };


  protected:
    static int NumberOf;

};

#endif
