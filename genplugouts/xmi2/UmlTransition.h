#ifndef _UMLTRANSITION_H
#define _UMLTRANSITION_H


#include "UmlBaseTransition.h"
#include "UmlStateItem.h"
#include <qcstring.h>

class FileOut;

class UmlTransition : public UmlBaseTransition, public UmlStateItem {
  public:
     UmlTransition(void * id, const QCString & n) : UmlBaseTransition(id, n) {
    }

    //target memorize this transition as incoming
    void memo_incoming_trans();

    //write the outgoing transition and memorize it in the upper level
    virtual void write(FileOut & out);

    //write the transition definition
    void write_it(FileOut & out);

    //indicate the incoming transition
    virtual void write_in(FileOut & out);

};

#endif
