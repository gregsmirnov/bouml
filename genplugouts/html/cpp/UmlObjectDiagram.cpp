
#include "UmlObjectDiagram.h"

QCString UmlObjectDiagram::sKind() {
  return "object diagram";
}

void UmlObjectDiagram::memo_ref() {
  diagrams.addElement(this);
  UmlDiagram::memo_ref();
}

void UmlObjectDiagram::ref_index()
{
  if (!diagrams.isEmpty())
    fw.write("<a href=\"objectdiagrams.html\" target = \"projectFrame\"><b> -Object Diagrams- </b></a>");
}

void UmlObjectDiagram::generate_index()
{
  UmlItem::generate_index(diagrams, "Object Diagram", "objectdiagrams");
}

Vector UmlObjectDiagram::diagrams;

