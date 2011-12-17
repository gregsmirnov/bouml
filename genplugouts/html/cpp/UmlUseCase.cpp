
#include "UmlUseCase.h"

#include "UmlUseCaseDiagram.h"
QCString UmlUseCase::sKind() {
  return "use case";
}

void UmlUseCase::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlItem::html(pfix, rank, "Use Case", level, "usecase");
    
  QCString ep = extensionPoints();
  
  if (! ep.isEmpty()) {
    fw.write("<p>Extension points:<br /><div class=\"sub\">");
    writeq(ep);
    fw.write("</div></p>\n");
  }

  UmlDiagram * d = associatedDiagram();
  
  if (d != 0) {
    fw.write("<p>Diagram : ");
    d->write();
    fw.write("</p>\n");
  }

  unload(FALSE, FALSE);
}

void UmlUseCase::memo_ref() {
  usecases.addElement(this);
  UmlItem::memo_ref();
}

void UmlUseCase::ref_index()
{
  if (!usecases.isEmpty())
    fw.write("<a href=\"usecases.html\" target = \"projectFrame\"><b> -Use Cases- </b></a>");
}

void UmlUseCase::generate_index()
{
  UmlItem::generate_index(usecases, "Use Cases", "usecases");
}

bool UmlUseCase::chapterp() {
  return TRUE;
}

Vector UmlUseCase::usecases;

