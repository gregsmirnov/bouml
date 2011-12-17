
#include "UmlDeploymentDiagram.h"

QCString UmlDeploymentDiagram::sKind() {
  return "deployment diagram";
}

void UmlDeploymentDiagram::memo_ref() {
  diagrams.addElement(this);
  UmlDiagram::memo_ref();
}

void UmlDeploymentDiagram::ref_index()
{
  if (!diagrams.isEmpty())
    fw.write("<a href=\"deploymentdiagrams.html\" target = \"projectFrame\"><b> -Deployment Diagrams- </b></a>");
}

void UmlDeploymentDiagram::generate_index()
{
  UmlItem::generate_index(diagrams, "Deployment Diagram", "deploymentdiagrams");
}

Vector UmlDeploymentDiagram::diagrams;

