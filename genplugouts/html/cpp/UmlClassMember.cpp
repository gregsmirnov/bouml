
#include "UmlClassMember.h"

void UmlClassMember::annotation_constraint() {
  if (! javaAnnotations().isEmpty()) {
    fw.write("<p> Java annotation(s) :</p><div class=\"sub\"><p>");
    writeq(javaAnnotations());
    fw.write("</p></div>");
  }
  
  if (! constraint().isEmpty()) {
    fw.write("<p> Constraint :</p><div class=\"sub\"><p>");
    writeq(constraint());
    fw.write("</p></div>");
  }
}

