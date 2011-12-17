#ifndef _UMLSTATE_H
#define _UMLSTATE_H


#include "UmlBaseState.h"
#include "UmlStateItem.h"
#include <qcstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

class UmlState : public UmlBaseState, public UmlStateItem {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlState(void * id, const QCString & s) : UmlBaseState(id, s) {
       NumberOf += 1; 
    }

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);


  private:
    void importActivity(FileIn & in, Token & token);


  public:
    //  call at end of import : try to solve reference to operation
    
    virtual void solve(int context, QCString idref);

    static void init();

    //import the state [machine] starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

    static int numberOf() { return NumberOf; };


  protected:
    static int NumberOf;

};

#endif
