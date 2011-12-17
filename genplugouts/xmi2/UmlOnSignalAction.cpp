
#include "UmlOnSignalAction.h"
#include "FileOut.h"

void UmlOnSignalAction::write_signal(FileOut & out) {
  QCString sig;
  
  switch (_lang) {
  case Uml:
    sig = signal();
    break;
  case Cpp:
    sig = cppSignal();
    break;
  default:
    // java
    sig = javaSignal();
  }

  if (! sig.isEmpty()) {
    out.indent();
    out << "<signal xmi:type=\"uml:Signal\"";
    out.id_prefix(this, "SIGNAL_");
    out << " name=\"";
    out.quote(sig);
    out << "\"/>\n";
  }
}

