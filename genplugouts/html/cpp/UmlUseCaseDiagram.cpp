
#include "UmlUseCaseDiagram.h"

QCString UmlUseCaseDiagram::sKind() {
  return "use case diagram";
}

void UmlUseCaseDiagram::memo_ref() {
  diagrams.addElement(this);
  UmlDiagram::memo_ref();
}

void UmlUseCaseDiagram::ref_index()
{
  if (!diagrams.isEmpty())
    fw.write("<a href=\"usecasediagrams.html\" target = \"projectFrame\"><b> -Use Case Diagrams- </b></a>");
}

void UmlUseCaseDiagram::generate_index()
{
  UmlItem::generate_index(diagrams, "Use Case Diagram", "usecasediagrams");
}

Vector UmlUseCaseDiagram::diagrams;

