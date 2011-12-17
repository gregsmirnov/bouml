#ifndef _UMLFINALSTATE_H
#define _UMLFINALSTATE_H


#include "UmlBaseFinalState.h"

class FileOut;

class UmlFinalState : public UmlBaseFinalState {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlFinalState(void * id) : UmlBaseFinalState(id, "") {
    }

    virtual void write(FileOut & out);

    //not used at this level
    virtual const char * sKind() const;

};

#endif
