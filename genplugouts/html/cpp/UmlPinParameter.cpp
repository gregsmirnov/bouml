
#include "UmlPinParameter.h"

void UmlPinParameter::html(QCString pfix, unsigned int rank, unsigned int level) {
  if (isUnique()) {
    if (isException())
      fw.write("<p>Unique, exception</p>");
    else
      fw.write("<p>Read only</p>");
  }
  else if (isException())
    fw.write("<p>Exception</p>");

  fw.write("<p>Direction : ");
  write(direction());
  fw.write("</p>");

  if (effect() != noEffect) {
    fw.write("<p>Effect : ");
    write(effect());
    fw.write("</p>");
  }

  html_internal(pfix, rank, level);

}

