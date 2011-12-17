
#include "UmlFinalState.h"
#include "FileOut.h"

void UmlFinalState::write(FileOut & out) {
  out.indent();
  out << "<subvertex xmi:type=\"uml:FinalState\"";
  out.id(this);
  out << " name=\"Final\">\n";
  out.indent(+1);
  
  write_description_properties(out); 
     
  while (! _incoming_trans.isEmpty())
    _incoming_trans.take(0)->write_in(out);
  
  out.indent(-1);
  out.indent();
  out << "</subvertex>\n";
    
  unload(); 
}

const char * UmlFinalState::sKind() const {
  // to avoid warning
  return 0;
}

