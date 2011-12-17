
#include "UmlActivityPin.h"

QCString UmlActivityPin::sKind() {
  return "activity action pin";
}

void UmlActivityPin::html(QCString pfix, unsigned int rank, unsigned int level) {
  define();

  fw.write("<table><tr><td><div class=\"element\">Activity action pin <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>\n");

  UmlPinParameter::html(pfix, rank, level);

  unload(FALSE, FALSE);
}

