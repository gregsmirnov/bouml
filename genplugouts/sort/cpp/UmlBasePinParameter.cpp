
#include "UmlCom.h"
#include "UmlBasePinParameter.h"

bool UmlBasePinParameter::isUnique() {
    read_if_needed_();
    return _unique;
}

bool UmlBasePinParameter::set_IsUnique(bool v) {
    bool vv;
  
    if (set_it_(vv, v, setUniqueCmd)) {
      _unique = v;
      return TRUE;
    }
    else
      return FALSE;
}

bool UmlBasePinParameter::isException() {
    read_if_needed_();
    return _exception;
}

bool UmlBasePinParameter::set_IsException(bool v) {
    bool vv;
  
    if (set_it_(vv, v, replaceExceptionCmd)) {
      _exception = v;
      return TRUE;
    }
    else
      return FALSE;
}

aDirection UmlBasePinParameter::direction() {
    read_if_needed_();
    return _dir;
}

bool UmlBasePinParameter::set_Direction(aDirection v) {
    UmlCom::send_cmd(_identifier, setIdlDirCmd, (char) v);   if (UmlCom::read_bool()) {     _dir = v;     return TRUE;   }   else     return FALSE;
}

aParameterEffectKind UmlBasePinParameter::effect() {
    read_if_needed_();
    return _effect;
}

bool UmlBasePinParameter::set_Effect(aParameterEffectKind v) {
    UmlCom::send_cmd(_identifier, replaceParameterCmd, (char) v);   if (UmlCom::read_bool()) {     _effect = v;     return TRUE;   }   else     return FALSE;
}

void UmlBasePinParameter::read_uml_() {
    UmlBaseActivityObject::read_uml_();
    _unique = UmlCom::read_bool();
    _exception = UmlCom::read_bool();
    _stream = UmlCom::read_bool();
    _dir = (aDirection) UmlCom::read_char();
    _effect = (aParameterEffectKind) UmlCom::read_char();
}

