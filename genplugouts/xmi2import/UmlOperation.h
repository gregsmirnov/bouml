#ifndef _UMLOPERATION_H
#define _UMLOPERATION_H


#include "UmlBaseOperation.h"
#include <qcstring.h>

class FileIn;
class Token;
class UmlItem;

// This class manages 'operations', notes that the class 'UmlClassItem'
// is a mother class of all the class's children.
//
// You can modify it as you want (except the constructor)
class UmlOperation : public UmlBaseOperation {
  public:
    UmlOperation(void * id, const QCString & n) : UmlBaseOperation(id, n) {};

    static void init();

    //import the operation starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);


  private:
    void addException(Token & token, FileIn & in);

    void addParameter(Token & token, FileIn & in);


  public:
    //  call at end of import : try to solve types
    virtual void solve(int context, QCString idref);

};

#endif
