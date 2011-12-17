#ifndef _UMLBASEPINPARAMETER_H
#define _UMLBASEPINPARAMETER_H


#include "UmlActivityObject.h"
#include <qcstring.h>
#include "aDirection.h"
#include "aParameterEffectKind.h"

class UmlBasePinParameter : public UmlActivityObject {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBasePinParameter(void * id, const QCString & s) : UmlActivityObject(id, s) {
    }


  public:
    // return the  return the isUnique attribute
    bool isUnique();

    // set the isUnique attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IsUnique(bool v);

    // return the  return the isException attribute, exclusive with isStream
    bool isException();

    // set the isException attribute, exclusive with isStream
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IsException(bool v);

    // return the  return the isStream attribute
    bool isStream();

    // set the isStream attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IsStream(bool v);

    // return the direction
    aDirection direction();

    // set the direction
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Direction(aDirection v);

    // return the effect
    aParameterEffectKind effect();

    // set the effect
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Effect(aParameterEffectKind v);


  private:
    bool _unique : 1;

    bool _exception : 1;

    bool _stream : 1;

    int _dummy : 5;

    aDirection _dir : 8;

    aParameterEffectKind _effect : 8;


  protected:
    virtual void read_uml_();

};

#endif
