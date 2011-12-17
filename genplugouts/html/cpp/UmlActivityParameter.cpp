
#include "UmlActivityParameter.h"

QCString UmlActivityParameter::sKind() {
  return "activity parameter";
}

void UmlActivityParameter::html(QCString pfix, unsigned int rank, unsigned int level) {
  define();

  fw.write("<table><tr><td><div class=\"element\">Activity action pin <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>\n");

  QCString s = defaultValue();
  
  if (! s.isEmpty()) {
    fw.write("<p>Default value :</p><ul>");
  
    fw.write("<pre>\n");
    writeq(s);
    fw.write("</pre></ul>");
  }

  UmlPinParameter::html(pfix, rank, level);

  unload(FALSE, FALSE);
}

