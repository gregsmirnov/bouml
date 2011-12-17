
#include "UmlTransition.h"

QCString UmlTransition::sKind() {
  return "transition";
}

void UmlTransition::html(QCString, unsigned int, unsigned int) {
  fw.write("<table><tr><td><div class=\"element\">Transition  <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>");
  
  if (parent() == target()) {
    fw.write("<p>Self relation of ");
    parent()->write();
    fw.write((isExternal()) ? ", external</p>" : ", internal</p>");
  }
  else {
    fw.write("<p>From ");
    parent()->write();
    fw.write(" To ");
    target()->write();
    fw.write("</p>");
  }

  QCString s = description();

  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(s);
    fw.write("<br /></p>");
  }

  QCString scpp, sjava;
  
  s = trigger();
  scpp = cppTrigger();
  sjava = javaTrigger();

  if ((!s.isEmpty()) || (!scpp.isEmpty()) || (!sjava.isEmpty())) {
    fw.write("<p>Trigger :</p><ul>");
    
    if (!s.isEmpty()) {
      fw.write("<li>OCL : <pre>\n");
      writeq(s);
      fw.write("</pre></li>");
    }
    
    if (!scpp.isEmpty()) {
      fw.write("<li>C++ : <pre>\n");
      writeq(scpp);
      fw.write("</pre></li>");
    }
    
    if (!sjava.isEmpty()) {
      fw.write("<li>Java : <pre>\n");
      writeq(sjava);
      fw.write("</pre></li>");
    }
    
    fw.write("</ul>");
  }

  s = guard();
  scpp = cppGuard();
  sjava = javaGuard();

  if ((!s.isEmpty()) || (!scpp.isEmpty()) || (!sjava.isEmpty())) {
    fw.write("<p>Guard :</p><ul>");
    
    if (!s.isEmpty()) {
      fw.write("<li>OCL : <pre>\n");
      writeq(s);
      fw.write("</pre></li>");
    }
    
    if (!scpp.isEmpty()) {
      fw.write("<li>C++ : <pre>\n");
      writeq(scpp);
      fw.write("</pre></li>");
    }
    
    if (!sjava.isEmpty()) {
      fw.write("<li>Java : <pre>\n");
      writeq(sjava);
      fw.write("</pre></li>");
    }
    
    fw.write("</ul>");
  }

  s = activity();
  scpp = cppActivity();
  sjava = javaActivity();

  if ((!s.isEmpty()) || (!scpp.isEmpty()) || (!sjava.isEmpty())) {
    fw.write("<p>Activity :</p><ul>");
    
    if (!s.isEmpty()) {
      fw.write("<li>OCL : <pre>\n");
      writeq(s);
      fw.write("</pre></li>");
    }
    
    if (!scpp.isEmpty()) {
      fw.write("<li>C++ : <pre>\n");
      writeq(scpp);
      fw.write("</pre></li>");
    }
    
    if (!sjava.isEmpty()) {
      fw.write("<li>Java : <pre>\n");
      writeq(sjava);
      fw.write("</pre></li>");
    }
    
    fw.write("</ul>");
  }
 
  write_properties();

  unload(FALSE, FALSE);
}

