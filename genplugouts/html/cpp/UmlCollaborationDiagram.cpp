
#include "UmlCollaborationDiagram.h"

QCString UmlCollaborationDiagram::sKind() {
  return "collaboration diagram";
}

void UmlCollaborationDiagram::memo_ref() {
  diagrams.addElement(this);
  UmlDiagram::memo_ref();
}

void UmlCollaborationDiagram::ref_index()
{
  if (!diagrams.isEmpty())
    fw.write("<a href=\"collaborationdiagrams.html\" target = \"projectFrame\"><b> -Collaboration Diagrams- </b></a>");
}

void UmlCollaborationDiagram::generate_index()
{
  UmlItem::generate_index(diagrams, "Collaboration Diagram", "collaborationdiagrams");
}

Vector UmlCollaborationDiagram::diagrams;

