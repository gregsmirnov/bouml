
#include "UmlActivityPartition.h"

#include "UmlActivityDiagram.h"
bool UmlActivityPartition::chapterp() {
  return TRUE;
}

QCString UmlActivityPartition::sKind() {
  return "activity partition";
}

void UmlActivityPartition::html(QCString pfix, unsigned int rank, unsigned int level) {
  define();
  
  chapter("Activity partition", pfix, rank, "activitypartition", level);

  QCString s = description();
  
  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(s);
    fw.write("<br /></p>");
  }
  
  if (isDimension())
    fw.write((isExternal())
	     ? "<p>is dimension, is external</p>\n"
	     : "<p>is dimension</p>\n");
  else if (isExternal())
    fw.write("<p>is external</p>\n");
    
  if (represents() != 0) {
    fw.write("<p>represents ");
    represents()->write();
    fw.write("</p>");
  }
    
  write_dependencies();
    
  UmlDiagram * d = associatedDiagram();
  
  if (d != 0) {
    fw.write("<p>Diagram : ");
    d->write();
    fw.write("</p>\n");
  }

  write_properties();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

