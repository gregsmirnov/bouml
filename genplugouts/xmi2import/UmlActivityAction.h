#ifndef _UMLACTIVITYACTION_H
#define _UMLACTIVITYACTION_H


#include "UmlBaseActivityAction.h"
#include <qcstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

class UmlActivityAction : public UmlBaseActivityAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityAction(void * id, const QCString & s) : UmlBaseActivityAction(id, s) {
    }


  public:
    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    //import the body of the activity action starting by 'tk'
    
    virtual void import_it(FileIn & in, Token & token);

    //look at the token kind to call the function managing
    //this case or bypass the token
    
    virtual void import(FileIn & in, Token & token);


  private:
    void readCondition(FileIn & in, Token & token);

};

#endif
