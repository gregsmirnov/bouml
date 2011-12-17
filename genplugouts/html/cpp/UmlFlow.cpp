
#include "UmlActivityNode.h"
#include "UmlFlow.h"

QCString UmlFlow::sKind() {
  return "transition";
}

void UmlFlow::html(QCString, unsigned int, unsigned int) {
  fw.write("<table><tr><td><div class=\"element\">Flow  <b>");
  writeq(name());
  fw.write("</b></div></td></tr></table>");

  fw.write("<p>From ");
  parent()->write();
  fw.write(" To ");
  target()->write();
  fw.write("</p>");

  QCString s = description();

  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(s);
    fw.write("<br /></p>");
  }
  
  QCString scpp, sjava;
  
  s = weight();
  scpp = cppWeight();
  sjava = javaWeight();
  
  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Weight :</p><ul>");
    
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
  
  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
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

  s = selection();
  scpp = cppSelection();
  sjava = javaSelection();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Selection :</p><ul>");
    
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

  s = transformation();
  scpp = cppTransformation();
  sjava = javaTransformation();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Transformation :</p><ul>");
    
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

