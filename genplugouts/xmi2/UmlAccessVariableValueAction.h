#ifndef _UMLACCESSVARIABLEVALUEACTION_H
#define _UMLACCESSVARIABLEVALUEACTION_H


#include "UmlBaseAccessVariableValueAction.h"
#include <qcstring.h>

class FileOut;

class UmlAccessVariableValueAction : public UmlBaseAccessVariableValueAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAccessVariableValueAction(void * id, const QCString & s) : UmlBaseAccessVariableValueAction(id, s) {
    }

    void write_var_end(FileOut & out);

};

#endif
