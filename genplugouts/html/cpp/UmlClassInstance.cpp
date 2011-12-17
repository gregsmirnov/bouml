
#include "UmlCom.h"
#include "UmlClass.h"
#include "UmlAttribute.h"
#include "UmlRelation.h"
#include "UmlClassInstance.h"

QCString UmlClassInstance::sKind() {
  return "class instance";
}

void UmlClassInstance::html(QCString, unsigned int, unsigned int) {
  define();

  fw.write("<table><tr><td><div class=\"element\">Class instance  <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>");
  
  QCString s = description();

  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(s);
    fw.write("<br /></p>");
  }

  fw.write("<p>type :");
  type()->write();
  fw.write("</p>");
  
  QValueList<SlotAttribute> va;
  
  attributesValue(va);
  
  if (!va.isEmpty()) {
    fw.write("<p>attributes :</p><ul>\n");
    
    QValueList<SlotAttribute>::Iterator it;
    
    for (it = va.begin(); it != va.end(); ++it) {
      SlotAttribute & slot = *it;

      fw.write("<li>");
      slot.attribute->write();
      fw.write(" = ");
      writeq(slot.value);
      fw.write("</li>\n");
    }
    fw.write("</ul>");
  }
  
  QValueList<SlotRelation> vr;
  
  relationsValue(vr);
  
  if (!vr.isEmpty()) {
    fw.write("<p>relations :</p><ul>\n");
    
    QValueList<SlotRelation>::Iterator it;
    
    for (it = vr.begin(); it != vr.end(); ++it) {
      SlotRelation & slot = *it;

      fw.write("<li>");
      slot.relation->write();
      fw.write(" = ");
      slot.value->write();
      fw.write("</li>\n");
    }
    fw.write("</ul>");
  }

  write_properties();

  unload(FALSE, FALSE);
}

