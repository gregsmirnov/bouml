
#include "UmlStateDiagram.h"
#include "UmlOperation.h"
#include "UmlState.h"

QCString UmlState::sKind() {
  return (parent()->kind() == aClassView)
    ? "state machine" : "state";
}

void UmlState::html(QCString pfix, unsigned int rank, unsigned int level) {
  define();

  chapter((parent()->kind() == aClassView)
	  ? "StateMachine" : "State",
	  pfix, rank, "state", level);

  QCString s = description();
  
  if (!s.isEmpty()) {
    fw.write("<p>");
    writeq(description());
    fw.write("<br /></p>");
  }

  UmlState * ref = reference();
  
  if (ref != 0) {
    fw.write("<p>References ");
    ref->write();
    fw.write("</p>");
  }
  else {
    if (isActive())
      fw.write("<p>Active state</p>\n");
    
    UmlOperation * beh = specification();
    
    if (beh != 0) {
      fw.write("<p>Implements ");
      beh->write();
      fw.write("</p>");
    }
    
    QCString scpp, sjava;
    
    s = entryBehavior();
    scpp = cppEntryBehavior();
    sjava = javaEntryBehavior();
    
    if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
      fw.write("<p>Entry Behavior :</p><ul>");
      
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
    
    s = exitBehavior();
    scpp = cppExitBehavior();
    sjava = javaExitBehavior();
    
    if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
      fw.write("<p>Exit Behavior :</p><ul>");
      
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
    
    s = doActivity();
    scpp = cppDoActivity();
    sjava = javaDoActivity();
    
    if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
      fw.write("<p>Do activity :</p><ul>");
      
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
  }
  
  UmlStateDiagram * d = associatedDiagram();
  
  if (d != 0) {
    fw.write("<p>Diagram : ");
    d->write();
    fw.write("</p>");
  }

  write_properties();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

void UmlState::memo_ref() {
  states.addElement(this);
  UmlItem::memo_ref();
}

void UmlState::ref_index()
{
  if (!states.isEmpty())
    fw.write("<a href=\"states.html\" target = \"projectFrame\"><b> -States- </b></a>");
}

void UmlState::generate_index()
{
  UmlItem::generate_index(states, "States", "states");
}

bool UmlState::chapterp() {
  return TRUE;
}

Vector UmlState::states;

