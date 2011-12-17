
#include "UmlNode.h"
#include "FileOut.h"

bool UmlNode::write_if_needed(FileOut & out) {
  parent()->write(out); 
   
  out.indent(); 
  out << "<UML:Node name=\"";
  out.quote(name());
  out << '"'; 
  out.id(this); 
  out << " visibility=\"public\" isAbstract=\"false\" isActive=\"false\" >\n"; 
  out.indent(+1); 
  write_stereotype(out); 
  write_description_properties(out); 
  out.indent(-1); 
  out.indent(); 
  out << "</UML:Node>\n"; 
 
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
     
  for (unsigned i = 0; i != n; i += 1) 
    ch[i]->write_if_needed(out); 
 
  unload(); 
  
  return TRUE; 
}

void UmlNode::write(FileOut &) {
}

