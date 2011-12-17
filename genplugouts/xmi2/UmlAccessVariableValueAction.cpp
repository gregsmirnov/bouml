
#include "UmlAccessVariableValueAction.h"
#include "FileOut.h"

void UmlAccessVariableValueAction::write_var_end(FileOut & out) {
  write_end(out, TRUE);
  
  UmlItem * v = variable();
  
  if (v != 0) {
    out.indent();
    out << "<variable";
    out.idref(v);
    out << "/>\n";
  }

  write_close(out);
}

