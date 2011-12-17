
#include "UmlComponentDiagram.h"

QCString UmlComponentDiagram::sKind() {
  return "component diagram";
}

void UmlComponentDiagram::memo_ref() {
  diagrams.addElement(this);
  UmlDiagram::memo_ref();
}

void UmlComponentDiagram::ref_index()
{
  if (!diagrams.isEmpty())
    fw.write("<a href=\"componentdiagrams.html\" target = \"projectFrame\"><b> -Component Diagrams- </b></a>");
}

void UmlComponentDiagram::generate_index()
{
  UmlItem::generate_index(diagrams, "Component Diagram", "componentdiagrams");
}

Vector UmlComponentDiagram::diagrams;

