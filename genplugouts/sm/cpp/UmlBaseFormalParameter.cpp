
#include "UmlBaseFormalParameter.h"

#include "UmlCom.h"
#include "UmlBaseItem.h"
void UmlBaseFormalParameter::read_() {
  _name = UmlCom::read_string();
  _type = UmlCom::read_string();
  _default_value.type = (UmlClass *) UmlBaseItem::read_();
  if (_default_value.type == 0)
    _default_value.explicit_type = UmlCom::read_string();
  _extends.type = (UmlClass *) UmlBaseItem::read_();
  if (_extends.type == 0)
    _extends.explicit_type = UmlCom::read_string();
}

