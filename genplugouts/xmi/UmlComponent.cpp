
#include "UmlComponent.h"
#include "FileOut.h"

void UmlComponent::ref(FileOut & out) {
  out << "<UML:Component"; 
  out.idref(this); 
  out << "/>"; 
}

bool UmlComponent::write_if_needed(FileOut & out) {
  parent()->write(out); 
   
  out.indent(); 
  out << "<UML:Component name=\"";
  out.quote(name());
  out << '"'; 
  out.id(this); 
  out << " visibility=\"public\" isAbstract=\"false\" isActive=\"false\" >\n"; 
  out.indent(+1); 
  write_stereotype(out); 
  write_description_properties(out); 
  out.indent(-1); 
  out.indent(); 
  out << "</UML:Component>\n"; 
 
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
     
  for (unsigned i = 0; i != n; i += 1) 
    ch[i]->write_if_needed(out); 
 
  unload(); 
  
  return TRUE; 
}

void UmlComponent::write(FileOut &) {
}

