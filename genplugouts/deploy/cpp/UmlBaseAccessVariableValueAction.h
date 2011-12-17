#ifndef _UMLBASEACCESSVARIABLEVALUEACTION_H
#define _UMLBASEACCESSVARIABLEVALUEACTION_H


#include "UmlActivityAction.h"
#include <qcstring.h>

class UmlItem;

class UmlBaseAccessVariableValueAction : public UmlActivityAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAccessVariableValueAction(void * id, const QCString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the variable, may be an attribute or a relation
    UmlItem * variable();

    // set the variable
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Variable(UmlItem * v);


  private:
    UmlItem * _variable;


  protected:
    virtual void read_uml_();

};

#endif
