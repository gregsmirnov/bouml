
#include "UmlActivityActionClasses.h"

#include "UmlOperation.h"
QCString UmlSendObjectAction::sKind() {
  return "send object activity action";
}

void UmlSendObjectAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlUnmarshallAction::sKind() {
  return "unmarshall activity action";
}

void UmlUnmarshallAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlSendSignalAction::sKind() {
  return "send signal activity action";
}

QCString UmlBroadcastSignalAction::sKind() {
  return "broadcast signal activity action";
}

QCString UmlValueSpecificationAction::sKind() {
  return "value specification activity action";
}

void UmlValueSpecificationAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  QCString s = value();
  QCString scpp = cppValue();
  QCString sjava = javaValue();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Value :</p><ul>");
  
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

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlOpaqueAction::sKind() {
  return "opaque activity action";
}

void UmlOpaqueAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  QCString s = behavior();
  QCString scpp = cppBehavior();
  QCString sjava = javaBehavior();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Behavior :</p><ul>");
    
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

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlAcceptEventAction::sKind() {
  return "accept event activity action";
}

void UmlAcceptEventAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isUnmarshall()) {
    if (isTimeEvent())
      fw.write("<p>Unmarshall, event is a time event</p>");
    else
      fw.write("<p>Unmarshall</p>");
  }
  else if (isTimeEvent())
    fw.write("<p>Event is a time event</p>");
  
  QCString s = trigger();
  QCString scpp = cppTrigger();
  QCString sjava = javaTrigger();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
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

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlCallOperationAction::sKind() {
  return "call operation activity action";
}

void UmlCallOperationAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isSynchronous())
    fw.write("<p>Is synchronous</p>");
    
  if (operation() != 0){
    fw.write("<p>Operation : ");
    operation()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlCallBehaviorAction::sKind() {
  return "call behavior activity action";
}

void UmlCallBehaviorAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isSynchronous())
    fw.write("<p>Is synchronous</p>");
    
  if (behavior() != 0){
    fw.write("<p>Behavior : ");
    behavior()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlClearVariableValueAction::sKind() {
  return "clear variable value activity action";
}

QCString UmlReadVariableValueAction::sKind() {
  return "read variable value activity action";
}

QCString UmlWriteVariableValueAction::sKind() {
  return "write variable value activity action";
}

QCString UmlAddVariableValueAction::sKind() {
  return "add variable value activity action";
}

void UmlAddVariableValueAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isReplaceAll())
    fw.write("<p>Replace all</p>");
    
  if (variable() != 0){
    fw.write("<p>Variable : ");
    variable()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlRemoveVariableValueAction::sKind() {
  return "remove variable value activity action";
}

void UmlRemoveVariableValueAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isRemoveDuplicates())
    fw.write("<p>Remove duplicates</p>");
    
  if (variable() != 0){
    fw.write("<p>Variable : ");
    variable()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlAcceptCallAction::sKind() {
  return "accept call activity action";
}

void UmlAcceptCallAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();
  
  QCString s = trigger();
  QCString scpp = cppTrigger();
  QCString sjava = javaTrigger();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
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

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlReplyAction::sKind() {
  return "reply activity action";
}

void UmlReplyAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  QCString s = replyToCall();
  QCString scpp = cppReplyToCall();
  QCString sjava = javaReplyToCall();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>replyToCall :</p><ul>");
    
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

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlCreateObjectAction::sKind() {
  return "create object activity action";
}

void UmlCreateObjectAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (!classifier().isEmpty()){
    fw.write("<p>Classifier : ");
    writeq(classifier());
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlDestroyObjectAction::sKind() {
  return "destroy object activity action";
}

void UmlDestroyObjectAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isDestroyLinks()) {
    if (isDestroyOwnedObjects())
      fw.write("<p>Destroy links, destroy owned objects</p>");
    else
      fw.write("<p>Destroy links</p>");
  }
  else if (isDestroyOwnedObjects())
    fw.write("<p>Destroy owned objects</p>");

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlTestIdentityAction::sKind() {
  return "test identity activity action";
}

void UmlTestIdentityAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlRaiseExceptionAction::sKind() {
  return "raise exception activity action";
}

void UmlRaiseExceptionAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

QCString UmlReduceAction::sKind() {
  return "reduce activity action";
}

void UmlReduceAction::html(QCString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isOrdered())
    fw.write("<p>Ordered</p>");

  if (reducer() != 0){
    fw.write("<p>Reducer : ");
    reducer()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

