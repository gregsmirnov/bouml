
#include "UmlPinParameter.h"
#include "FileOut.h"

void UmlPinParameter::solve_output_flows() {
  ControlOrData v = (isControlType()) ? IsControl : IsData;
  const QVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1) {
    UmlFlow * f = dynamic_cast<UmlFlow *>(ch[i]);

    if ((f != 0) && (f->control_or_data() == Unset))
      f->set_control_or_data(v);
  }

}

void UmlPinParameter::write_dir(FileOut & out) {
  out << " direction =\"";
  if (_pk_prefix)
    out << "pk_";
  switch (direction()) {
  case InputOutputDirection: out << "inout\""; break;
  case OutputDirection: out << "out\""; break;
  default: out << "in\"";
  }
}

void UmlPinParameter::write_flags(FileOut & out) {
  if (isUnique())
    out << " isUnique=\"true\"";
  if (isException())
    out << " isException=\"true\"";
  if (isStream())
    out << " isStream=\"true\"";
}

