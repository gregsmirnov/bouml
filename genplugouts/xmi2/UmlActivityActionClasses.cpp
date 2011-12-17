
#include "UmlActivityActionClasses.h"
#include "FileOut.h"

#include "UmlOperation.h"
#include "UmlActivity.h"
void UmlSendObjectAction::write(FileOut & out) {
  write_begin(out, "SendObjectAction");
  write_end(out);

}

void UmlUnmarshallAction::write(FileOut & out) {
  write_begin(out, "UnmarshallAction");
  write_end(out);
}

void UmlSendSignalAction::write(FileOut & out) {
  write_begin(out, "SendSignalAction");
  write_end(out, TRUE);
  write_signal(out);
  write_close(out);
}

void UmlBroadcastSignalAction::write(FileOut & out) {
  write_begin(out, "BroadcastSignalAction");
  write_end(out, TRUE);
  write_signal(out);
  write_close(out);
}

void UmlValueSpecificationAction::write(FileOut & out) {
  write_begin(out, "ValueSpecificationAction");
  
  QCString val;
  
  switch (_lang) {
  case Uml:
    val = value();
    break;
  case Cpp:
    val = cppValue();
    break;
  default:
    // Java
    val = javaValue();
  }
  if (! val.isEmpty())
    out.ref(activity()->add_opaque_expression(val, this),
	    "value", "OPAQUE_EXPRESSION_");

  write_end(out);
}

void UmlOpaqueAction::write(FileOut & out) {
  write_begin(out, "OpaqueAction");
  write_end(out, TRUE);
  
  QCString body;
  
  switch(_lang) {
  case Uml:
    body = behavior();
    break;
  case Cpp:
    body = cppBehavior();
    break;
  default:
    // Java
    body = javaBehavior();
  }

  if (!body.isEmpty()) {
    out.indent();
    out << "<body>";
    out.quote(body);
    out << "</body>\n";
  }

  write_close(out);

}

void UmlAcceptEventAction::write(FileOut & out) {
  write_begin(out, "AcceptEventAction");
  
  if (isUnmarshall())
    out << " isUnmarshall=\"true\"";
  
  QCString trig;
  
  switch (_lang) {
  case Uml:
    trig = trigger();
    break;
  case Cpp:
    trig = cppTrigger();
    break;
  default:
    // java
    trig = javaTrigger();
  }

  if (! trig.isEmpty()) {
    out.ref(this, "trigger", "TRIGGER_");
    write_end(out);
    
    out.indent();
    out << "<trigger xmi:type=\"uml:Trigger\"";
    out.id_prefix(this, "TRIGGER_");
    out << " name=\"";
    out.quote(trig);
    out << "\"/>\n";
  }
  else
    write_end(out);

}

void UmlCallOperationAction::write(FileOut & out) {
  write_begin(out, "CallOperationAction");
  write_end(out, TRUE);
  
  UmlOperation * op = operation();
  
  if (op != 0) {
    out.indent();
    out << "<operation";
    out.idref(op);
    out << "/>\n";
  }

  write_close(out);

}

void UmlCallBehaviorAction::write(FileOut & out) {
  write_begin(out, "CallBehaviorAction");
  write_end(out, TRUE);
  
  UmlItem * b = behavior();
  
  if (b != 0) {
    out.indent();
    out << "<behavior";
    out.idref(b);
    out << "/>\n";
  }

  write_close(out);

}

void UmlClearVariableValueAction::write(FileOut & out) {
  write_begin(out, "ClearVariableAction");
  write_var_end(out);
}

void UmlReadVariableValueAction::write(FileOut & out) {
  write_begin(out, "ReadVariableAction");
  write_var_end(out);
}

void UmlWriteVariableValueAction::write(FileOut & out) {
  write_begin(out, "WriteVariableAction");
  write_var_end(out);

}

void UmlAddVariableValueAction::write(FileOut & out) {
  write_begin(out, "AddVariableValueAction");
  
  if (isReplaceAll())
    out << " isReplaceAll=\"true\"";
  
  write_var_end(out);
}

void UmlRemoveVariableValueAction::write(FileOut & out) {
  write_begin(out, "RemoveVariableValueAction");

  if (isRemoveDuplicates())
    out << " isRemoveDuplicates=\"true\"";

  write_var_end(out);
}

void UmlAcceptCallAction::write(FileOut & out) {
  write_begin(out, "AcceptCallAction");
  out << " isUnmarshall=\"true\"";

  QCString trig;
  
  switch (_lang) {
  case Uml:
    trig = trigger();
    break;
  case Cpp:
    trig = cppTrigger();
    break;
  default:
    // java
    trig = javaTrigger();
  }

  if (! trig.isEmpty()) {
    out.ref(this, "trigger", "TRIGGER_");
    write_end(out);
    
    out.indent();
    out << "<trigger xmi:type=\"uml:Trigger\"";
    out.id_prefix(this, "TRIGGER_");
    out << " name=\"";
    out.quote(trig);
    out << "\"/>\n";
  }
  else
    write_end(out);

}

void UmlReplyAction::write(FileOut & out) {
  write_begin(out, "ReplyAction");
  
  QCString trig;
  
  switch (_lang) {
  case Uml:
    trig = replyToCall();
    break;
  case Cpp:
    trig = cppReplyToCall();
    break;
  default:
    // java
    trig = javaReplyToCall();
  }

  if (! trig.isEmpty()) {
    out.ref(this, "replyToCall", "TRIGGER_");
    write_end(out);
    
    out.indent();
    out << "<trigger xmi:type=\"uml:Trigger\"";
    out.id_prefix(this, "TRIGGER_");
    out << " name=\"";
    out.quote(trig);
    out << "\"/>\n";
  }
  else
    write_end(out);
}

void UmlCreateObjectAction::write(FileOut & out) {
  write_begin(out, "CreateObjectAction");
  write_end(out);

}

void UmlDestroyObjectAction::write(FileOut & out) {
  write_begin(out, "DestroyObjectAction");
  if (isDestroyLinks())
    out << " isDestroyLinks=\"true\"";
  if (isDestroyOwnedObjects())
    out << " isDestroyOwnedObjects=\"true\"";
  write_end(out);
}

void UmlTestIdentityAction::write(FileOut & out) {
  write_begin(out, "TestIdentityAction");
  write_end(out);

}

void UmlRaiseExceptionAction::write(FileOut & out) {
  write_begin(out, "RaiseExceptionAction");
  write_end(out);

}

void UmlReduceAction::write(FileOut & out) {
  write_begin(out, "ReduceAction");
  
  if (isOrdered())
    out << " isOrdered=\"true\"";
  
  write_end(out, TRUE);

  UmlItem * r = reducer();
  
  if (r != 0) {
    out.indent();
    out << "<reducer";
    out.idref(r);
    out << "/>\n";
  }

  write_close(out);
}

