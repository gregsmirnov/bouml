
#include "UmlBaseActualParameter.h"
#include "UmlClass.h"

#include "UmlCom.h"
UmlClass * UmlBaseActualParameter::superClass() const { return _superClass; }

unsigned UmlBaseActualParameter::rank() const { return _rank; }


const UmlTypeSpec & UmlBaseActualParameter::value() const { return _value; }

void UmlBaseActualParameter::read_() {
  _superClass = (UmlClass *) UmlBaseItem::read_();	// cannot be 0
  _rank = UmlCom::read_unsigned();
  _value.type = (UmlClass *) UmlBaseItem::read_();
  if (_value.type == 0)
    _value.explicit_type = UmlCom::read_string();
}

