
#include "UmlActivityDiagram.h"

void UmlActivityDiagram::memo_ref() {
  diagrams.addElement(this);
  UmlDiagram::memo_ref();
}

QCString UmlActivityDiagram::sKind() {
  return "activity diagram";
}

void UmlActivityDiagram::ref_index()
{
  if (!diagrams.isEmpty())
    fw.write("<a href=\"activitydiagrams.html\" target = \"projectFrame\"><b> -Activity Diagrams- </b></a>");
}

void UmlActivityDiagram::generate_index()
{
  UmlItem::generate_index(diagrams, "Activity Diagram", "activitydiagrams");
}

Vector UmlActivityDiagram::diagrams;

