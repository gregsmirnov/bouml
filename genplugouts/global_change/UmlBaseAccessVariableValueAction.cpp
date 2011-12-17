
#include "UmlCom.h"
#include "UmlBaseAccessVariableValueAction.h"
#include "UmlItem.h"

UmlItem * UmlBaseAccessVariableValueAction::variable() {
  read_if_needed_();
  return _variable;
}

bool UmlBaseAccessVariableValueAction::set_Variable(UmlItem * v) {
  UmlCom::send_cmd(_identifier, setDefCmd, ((UmlBaseItem *) v)->_identifier);   if (UmlCom::read_bool()) {     _variable = v;     return TRUE;   }   else     return FALSE;
}

void UmlBaseAccessVariableValueAction::read_uml_() {
  UmlBaseActivityAction::read_uml_();
  _variable = UmlBaseItem::read_();
}

