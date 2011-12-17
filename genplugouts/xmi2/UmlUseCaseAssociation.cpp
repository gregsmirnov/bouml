
#include "UmlUseCaseAssociation.h"
#include "FileOut.h"
#include "UmlItem.h"

#include "UmlClass.h"
#include "UmlUseCase.h"
#include "UmlUseCaseReference.h"
void UmlUseCaseAssociation::write(FileOut & out, UmlItem * d, int rank, const char * k) {
  
  out.indent();
  out << '<' << k << " xmi:type=\"uml:Association\"";
  out.id_prefix(d, "UCAC", rank);
  if (! name().isEmpty()){
    out << " name=\"";
    out.quote(name());
    out << "\">\n";
  }
  else
    out << ">\n";
  out.indent(+1); 
  
  out.indent();
  out << "<ownedEnd xmi:type=\"uml:Property\"";
  out.id_prefix(d, "AC", rank);
  out.ref(actor(), "type");
  out.ref(d, "association", "UCAC", rank);
  out << "/>\n";
  
  out.indent();
  out << "<ownedEnd xmi:type=\"uml:Property\"";
  out.id_prefix(d, "UC", rank);
  out.ref(useCase()->useCase(), "type");
  out.ref(d, "association", "UCAC", rank);
  out << "/>\n";

  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
}

