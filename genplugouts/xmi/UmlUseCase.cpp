
#include "UmlUseCase.h"
#include "FileOut.h"

void UmlUseCase::ref(FileOut & out) {
  out << "<UML:UseCase"; 
  out.idref(this); 
  out << "/>"; 
}

bool UmlUseCase::write_if_needed(FileOut & out) {
  // even if the use case doesn't have children 
  parent()->write(out); 
   
  out.indent(); 
  out << "<UML:UseCase name=\"";
  out.quote(name());
  out << '"'; 
  out.id(this); 
  out << " visibility=\"public\" isAbstract=\"false\" isActive=\"false\" >\n"; 
  out.indent(+1); 
  write_stereotype(out); 
  write_description_properties(out); 
     
  const QVector<UmlItem> ch = children(); 
  unsigned n = ch.size(); 
  bool used = FALSE; 
  bool haveRel= FALSE; 
     
  for (unsigned i = 0; i != n; i += 1) 
    if (ch[i]->kind() != aNcRelation) 
      used |= ch[i]->write_if_needed(out); 
    else 
      haveRel = TRUE; 
       
  if (used) { 
    out.indent(-1); 
    out.indent(); 
    out << "</UML:Namespace.ownedElement>\n"; 
  } 
   
  out.indent(-1); 
  out.indent(); 
  out << "</UML:UseCase>\n"; 
 
  if (haveRel) { 
    used = FALSE; 
    
    for (unsigned i = 0; i != n; i += 1)  
      if (ch[i]->kind() == aNcRelation) 
        used |= ch[i]->write_if_needed(out); 
       
    if (used) { 
      out.indent(-1); 
      out.indent(); 
      out << "</UML:Namespace.ownedElement>\n"; 
    } 
  }
 
  unload(); 
  
  return TRUE; 
}

void UmlUseCase::write(FileOut & out) {
  if (! _written) { 
    _written = TRUE; 
 
    // parent already written 
     
    out.indent(); 
    out << "<UML:Namespace.ownedElement>\n"; 
    out.indent(+1); 
  } 
}

