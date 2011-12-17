
#include "UmlCom.h"
#include "FlowBehavior.h"

void FlowBehavior::read() {
    weight = UmlCom::read_string();
    guard = UmlCom::read_string();
    selection = UmlCom::read_string();
    transformation = UmlCom::read_string();
}

void FlowBehavior::unload() {
    weight = 0;
    guard = 0;
    selection = 0;
    transformation = 0;
}

