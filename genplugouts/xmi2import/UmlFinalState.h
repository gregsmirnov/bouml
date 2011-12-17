#ifndef _UMLFINALSTATE_H
#define _UMLFINALSTATE_H


#include "UmlBaseFinalState.h"

class FileIn;
class Token;
class UmlItem;

class UmlFinalState : public UmlBaseFinalState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlFinalState(void * id) : UmlBaseFinalState(id, "") {
    }

    static void init();

    //import the final state starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);

};

#endif
