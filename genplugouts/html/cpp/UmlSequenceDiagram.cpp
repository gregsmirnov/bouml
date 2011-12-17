
#include "UmlSequenceDiagram.h"

QCString UmlSequenceDiagram::sKind() {
  return "sequence diagram";
}

void UmlSequenceDiagram::memo_ref() {
  diagrams.addElement(this);
  UmlDiagram::memo_ref();
}

void UmlSequenceDiagram::ref_index()
{
  if (!diagrams.isEmpty())
    fw.write("<a href=\"sequencediagrams.html\" target = \"projectFrame\"><b> -Sequence Diagrams- </b></a>");
}

void UmlSequenceDiagram::generate_index()
{
  UmlItem::generate_index(diagrams, "Sequence Diagram", "sequencediagrams");
}

Vector UmlSequenceDiagram::diagrams;

