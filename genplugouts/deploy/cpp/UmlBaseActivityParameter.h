#ifndef _UMLBASEACTIVITYPARAMETER_H
#define _UMLBASEACTIVITYPARAMETER_H


#include "UmlPinParameter.h"
#include "anItemKind.h"
#include <qcstring.h>

class UmlActivityParameter;
class UmlActivity;

class UmlBaseActivityParameter : public UmlPinParameter {
  public:
    //  returns a new activity parameter named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlActivityParameter * create(UmlActivity * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseActivityParameter(void * id, const QCString & s) : UmlPinParameter(id, s) {
    }


  public:
    // return the default value
    const QCString & defaultValue();

    // set the default value
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_DefaultValue(const char * v);

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QCString _default_value;


  protected:
    virtual void read_uml_();

};

#endif
