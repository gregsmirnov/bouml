
#include "UmlCom.h"
#include "StateBehavior.h"

void StateBehavior::read() {
  on_entry = UmlCom::read_string();
  on_exit = UmlCom::read_string();
  do_activity = UmlCom::read_string();
}

void StateBehavior::unload() {
  on_entry = 0;
  on_exit = 0;
  do_activity = 0;
}

