
#include "UmlCom.h"
#include "TransitionBehavior.h"

void TransitionBehavior::read() {
  trigger = UmlCom::read_string();
  guard = UmlCom::read_string();
  activity = UmlCom::read_string();
}

void TransitionBehavior::unload() {
  trigger = 0;
  guard = 0;
  activity = 0;
}

