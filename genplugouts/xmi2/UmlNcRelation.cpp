
#include "UmlNcRelation.h"
#include "FileOut.h"

void UmlNcRelation::write(FileOut & out) {
  if ((relationKind() == aDependency) &&
      (parent()->kind() == anUseCase) &&
      (target()->kind() == anUseCase)) {
    const char * t;
    const char * r;
    
    if (stereotype() == "include") {
      t = "Include";
      r = "addition";
    }
    else if (stereotype() == "extend") {
      t = "Extend";
      r = "extendedCase";
    }
    else {
      write(out, TRUE);
      return;
    }
      
    out.indent();
    out << "<" << stereotype() << " xmi:type=\"uml:" << t << "\"";
    out.id(this);
    out.ref(target(), r);
    out << ">\n";
    out.indent(+1); 
    write_description_properties(out);
    out.indent(-1);
    out.indent();
    out << "</" << stereotype() << ">\n";
  }
  else
    write(out, TRUE);
}

void UmlNcRelation::write(FileOut & out, bool inside) {
  if (relationKind() == aGeneralisation) {
    if (inside)
      write_generalization(out);
  }
  else {
    if (inside)
      parent()->memo_relation(this);
    else
      write_dependency(out);
  }
}

void UmlNcRelation::write_generalization(FileOut & out) {
  out.indent();
  out << "<generalization xmi:type=\"uml:Generalization\"";
  out.id(this);
  out.ref(target(), "general");
  out << "/>\n";
}

void UmlNcRelation::write_dependency(FileOut & out) {
  const char * k = (_uml_20) ? "ownedElement" : "packagedElement";

  out.indent();
  out << '<' << k <<
    (((parent()->kind() == anArtifact) && (stereotype() == "manifest"))
     ?  " xmi:type=\"uml:Manifestation\"" : " xmi:type=\"uml:Dependency\"");
  out.id(this);
  out.ref(parent(), "client");
  out.ref(target(), "supplier");
  out << ">\n";
  out.indent(+1); 
  write_description_properties(out);
  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";
}

