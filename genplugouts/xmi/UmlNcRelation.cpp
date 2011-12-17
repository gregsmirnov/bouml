
#include "UmlNcRelation.h"
#include "FileOut.h"

bool UmlNcRelation::write_if_needed(FileOut & out) {
  parent()->write(out);
 
  if (relationKind() == aGeneralisation)
    write_generalization(out); 
  else
    write_dependency(out); 
 
  return TRUE; 
}

void UmlNcRelation::write_generalization(FileOut & out) {
  out.indent(); 
  out << "<UML:Generalization"; 
  out.id(this); 
  out << " isSpecification=\"false\">\n"; 
  out.indent(); 
  out << "\t<UML:Generalization.child>";
  parent()->ref(out);
  out << "</UML:Generalization.child>\n"; 
  out.indent(); 
  out << "\t<UML:Generalization.parent>";
  target()->ref(out);
  out << "</UML:Generalization.parent>\n"; 
  out.indent(+1); 
  write_stereotype(out);  
  write_description_properties(out);
  out.indent(-1);
  out.indent(); 
  out << "</UML:Generalization>\n"; 
}

void UmlNcRelation::write_dependency(FileOut & out) {
  out.indent(); 
  out << "<UML:Dependency"; 
  out.id(this); 
  if (!name().isEmpty() && (((const char *) name())[0] != '<')) 
    out << " name = \"" << name() << "\""; 
  out << " visibility = \"private\" isSpecification=\"false\" client=\""; 
  out.ref(parent()); 
  out << "\" supplier=\""; 
  out.ref(target()); 
  out << "\">\n"; 
  out.indent(+1);  
  write_stereotype(out);   
  write_description_properties(out);
  out.indent(-1); 
  out.indent(); 
  out << "</UML:Dependency>\n"; 
}

