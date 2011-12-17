
#include "UmlStateDiagram.h"

void UmlStateDiagram::memo_ref() {
  diagrams.addElement(this);
  UmlDiagram::memo_ref();
}

QCString UmlStateDiagram::sKind() {
  return "state diagram";
}

void UmlStateDiagram::ref_index()
{
  if (!diagrams.isEmpty())
    fw.write("<a href=\"statediagrams.html\" target = \"projectFrame\"><b> -State Diagrams- </b></a>");
}

void UmlStateDiagram::generate_index()
{
  UmlItem::generate_index(diagrams, "State Diagram", "statediagrams");
}

Vector UmlStateDiagram::diagrams;

