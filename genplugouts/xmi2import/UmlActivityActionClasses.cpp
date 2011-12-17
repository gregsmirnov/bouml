
#include "UmlActivityActionClasses.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlCom.h"
#include "Trigger.h"
void UmlSendObjectAction::init()
{
  declareFct("node", "uml:SendObjectAction", &importIt);
  declareFct("containednode", "uml:SendObjectAction", &importIt);
}

void UmlSendObjectAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aSendObjectAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlSendObjectAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create send object action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlUnmarshallAction::init()
{
  declareFct("node", "uml:UnmarshallAction", &importIt);
  declareFct("containednode", "uml:UnmarshallAction", &importIt);
}

void UmlUnmarshallAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anUnmarshallAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlUnmarshallAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create unmarshall action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlSendSignalAction::init()
{
  declareFct("node", "uml:SendSignalAction", &importIt);
  declareFct("containednode", "uml:SendSignalAction", &importIt);
}

void UmlSendSignalAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aSendSignalAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlSendSignalAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create send signal action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlBroadcastSignalAction::init()
{
  declareFct("node", "uml:BroadcastSignalAction", &importIt);
  declareFct("containednode", "uml:BroadcastSignalAction", &importIt);
}

void UmlBroadcastSignalAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aBroadcastSignalAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlBroadcastSignalAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create broadcast signal action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlValueSpecificationAction::init()
{
  declareFct("node", "uml:ValueSpecificationAction", &importIt);
  declareFct("containednode", "uml:ValueSpecificationAction", &importIt);
}

void UmlValueSpecificationAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aValueSpecificationAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlValueSpecificationAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create value specification action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    QCString v = token.valueOf("value");
    
    if (! v.isEmpty())
      a->setValue(v);
    
    a->import_it(in, token);
  }
}

void UmlValueSpecificationAction::setValue(QCString idref) {
  QMap<QCString, QCString>::Iterator it = OpaqueDefs.find(idref);
  
  if (it != OpaqueDefs.end())
    set_Value(*it);
  else
    Unresolved::addRef(this, idref);
}

void UmlValueSpecificationAction::solve(QCString idref) {
  QMap<QCString, QCString>::Iterator it = OpaqueDefs.find(idref);
  
  if (it != OpaqueDefs.end())
    set_Value(*it);
  else if (!FileIn::isBypassedId(idref))
    UmlCom::trace("value specification action : unknown opaque definition reference '" + idref + "'<br>");

}

void UmlOpaqueAction::init()
{
  declareFct("node", "uml:OpaqueAction", &importIt);
  declareFct("containednode", "uml:OpaqueAction", &importIt);
  
  declareFct("node", "uml:Action", &importIt);
  declareFct("containednode", "uml:Action", &importIt);
}

void UmlOpaqueAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anOpaqueAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlOpaqueAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create opaque action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	if (token.what() == "body")
	  a->set_Behavior(in.body("body"));
	else
	  a->import(in, token);
      }
    }
  }
}

void UmlAcceptEventAction::init()
{
  declareFct("node", "uml:AcceptEventAction", &importIt);
  declareFct("containednode", "uml:AcceptEventAction", &importIt);
}

void UmlAcceptEventAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anAcceptEventAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlAcceptEventAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create accept event action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (token.valueOf("isunmarshall") == "true")
      a->set_isUnmarshall(TRUE);
    
    if (!(s = token.valueOf("trigger")).isEmpty()) {
      QCString tr = Trigger::get(s);
      
      if (!tr.isNull())
	a->set_Trigger(tr);
      else
	Unresolved::addRef(a, s);
    }
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	if (token.what() == "trigger") {
	  QCString tr_name;
	  QCString tr_ref;
	  
	  Trigger::add(in, token, tr_name, tr_ref);
      
	  if (!tr_name.isNull())
	    a->set_Trigger(tr_name);
	  else
	    Unresolved::addRef(a, tr_ref);
	}
	else
	  a->import(in, token);
      }
    }
  }
}

void UmlAcceptEventAction::solve(QCString idref) {
  QCString tr = Trigger::get(idref);
  
  if (tr.isNull()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("accept event activity action : unknown trigger reference '" + idref + "'<br>");
  }
  else
    set_Trigger(tr);
}

void UmlCallOperationAction::init()
{
  declareFct("node", "uml:CallOperationAction", &importIt);
  declareFct("containednode", "uml:CallOperationAction", &importIt);
}

void UmlCallOperationAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aCallOperationAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlCallOperationAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create call operation action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (!(s=token.valueOf("operation")).isEmpty())
      a->setOperation(s);
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if (s == "operation") {
	  a->setOperation(token.xmiIdref());
	  if (! token.closed())
	    in.finish("operation");
	}
	else
	  a->import(in, token);
      }
    }
  }
}

void UmlCallOperationAction::setOperation(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end())
    Unresolved::addRef(this, idref);
  else if ((*it)->kind() == anOperation)
    set_Operation((UmlOperation *) *it);
}

void UmlCallOperationAction::solve(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("call operation action : unknown operation reference '" + idref + "'<br>");
  }
  else if ((*it)->kind() == anOperation)
    set_Operation((UmlOperation *) *it);
}

void UmlCallBehaviorAction::init()
{
  declareFct("node", "uml:CallBehaviorAction", &importIt);
  declareFct("containednode", "uml:CallBehaviorAction", &importIt);
}

void UmlCallBehaviorAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aCallBehaviorAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlCallBehaviorAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create call behavior action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (!(s=token.valueOf("behavior")).isEmpty())
      a->setBehavior(s);
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if (s == "behavior") {
	  a->setBehavior(token.xmiIdref());
	  if (! token.closed())
	    in.finish("behavior");
	}
	else
	  a->import(in, token);
      }
    }
  }
}

void UmlCallBehaviorAction::setBehavior(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end())
    Unresolved::addRef(this, idref);
  else {
    switch ((*it)->kind()) {
    case anActivity:
    case aState:
      set_Behavior(*it);
    default:
      break;
    }
  }

}

void UmlCallBehaviorAction::solve(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("call behavior action : unknown behavior reference '" + idref + "'<br>");
  }
  else {
    switch ((*it)->kind()) {
    case anActivity:
    case aState:
      set_Behavior(*it);
    default:
      break;
    }
  }
}

void UmlClearVariableValueAction::init()
{
  declareFct("node", "uml:ClearVariableAction", &importIt);
  declareFct("containednode", "uml:ClearVariableAction", &importIt);
}

void UmlClearVariableValueAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aClearVariableValueAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlClearVariableValueAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create clear variable value action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlReadVariableValueAction::init()
{
  declareFct("node", "uml:ReadVariableAction", &importIt);
  declareFct("containednode", "uml:ReadVariableAction", &importIt);
}

void UmlReadVariableValueAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aReadVariableValueAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlReadVariableValueAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create read variable action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlWriteVariableValueAction::init()
{
  declareFct("node", "uml:WriteVariableAction", &importIt);
  declareFct("containednode", "uml:WriteVariableAction", &importIt);
}

void UmlWriteVariableValueAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aWriteVariableValueAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlWriteVariableValueAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create write variable value action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlAddVariableValueAction::init()
{
  declareFct("node", "uml:AddVariableValueAction", &importIt);
  declareFct("containednode", "uml:AddVariableValueAction", &importIt);
}

void UmlAddVariableValueAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anAddVariableValueAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlAddVariableValueAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create add variable action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlRemoveVariableValueAction::init()
{
  declareFct("node", "uml:RemoveVariableValueAction", &importIt);
  declareFct("containednode", "uml:RemoveVariableValueAction", &importIt);
}

void UmlRemoveVariableValueAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aRemoveVariableValueAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlRemoveVariableValueAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create remove variable action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    a->import_it(in, token);
  }
}

void UmlAcceptCallAction::init()
{
  declareFct("node", "uml:AcceptCallAction", &importIt);
  declareFct("containednode", "uml:AcceptCallAction", &importIt);
}

void UmlAcceptCallAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anAcceptCallAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlAcceptCallAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create accept call action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (!(s = token.valueOf("trigger")).isEmpty()) {
      QCString tr = Trigger::get(s);
      
      if (!tr.isNull())
	a->set_Trigger(tr);
      else
	Unresolved::addRef(a, s);
    }
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	if (token.what() == "trigger") {
	  QCString tr_name;
	  QCString tr_ref;
	  
	  Trigger::add(in, token, tr_name, tr_ref);
      
	  if (!tr_name.isNull())
	    a->set_Trigger(tr_name);
	  else
	    Unresolved::addRef(a, tr_ref);
	}
	else if (token.what() == "isunmarshall") {
	  // not memorized : always true
	  if (! token.closed())
	    in.finish(token.what());
	}
	else
	  a->import(in, token);
      }
    }
  }
}

void UmlAcceptCallAction::solve(QCString idref) {
  QCString tr = Trigger::get(idref);
  
  if (tr.isNull()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("accept call activity action : unknown trigger reference '" + idref + "'<br>");
  }
  else
    set_Trigger(tr);
}

void UmlReplyAction::init()
{
  declareFct("node", "uml:ReplyAction", &importIt);
  declareFct("containednode", "uml:ReplyAction", &importIt);
}

void UmlReplyAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aReplyAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlReplyAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create reply action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (!(s = token.valueOf("trigger")).isEmpty()) {
      QCString tr = Trigger::get(s);
      
      if (!tr.isNull())
	a->set_ReplyToCall(tr);
      else
	Unresolved::addRef(a, s);
    }
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	if (token.what() == "trigger") {
	  QCString tr_name;
	  QCString tr_ref;
	  
	  Trigger::add(in, token, tr_name, tr_ref);
      
	  if (!tr_name.isNull())
	    a->set_ReplyToCall(tr_name);
	  else
	    Unresolved::addRef(a, tr_ref);
	}
	else
	  a->import(in, token);
      }
    }
  }
}

void UmlReplyAction::solve(QCString idref) {
  QCString tr = Trigger::get(idref);
  
  if (tr.isNull()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("reply activity action : unknown trigger reference '" + idref + "'<br>");
  }
  else
    set_ReplyToCall(tr);
}

void UmlCreateObjectAction::init()
{
  declareFct("node", "uml:CreateObjectAction", &importIt);
  declareFct("containednode", "uml:CreateObjectAction", &importIt);
}

void UmlCreateObjectAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aCreateObjectAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlCreateObjectAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create create object action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    s = token.valueOf("classifier");
        
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	if (token.what() == "classifier") {
	  s = token.xmiIdref();
	  if (! token.closed())
	    in.finish(token.what());
	}
	else
	  a->import(in, token);
      }
    }
    
    if (!s.isEmpty()) {
      QMap<QCString, UmlItem *>::Iterator it = All.find(s);
      
      if (it == All.end())
	Unresolved::addRef(a, s);
      else
	a->set_Classifier((*it)->name());
    }
  }
}

void UmlCreateObjectAction::solve(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
      
  if (it != All.end())
    set_Classifier((*it)->name());
}

void UmlDestroyObjectAction::init()
{
  declareFct("node", "uml:DestroyObjectAction", &importIt);
  declareFct("containednode", "uml:DestroyObjectAction", &importIt);
}

void UmlDestroyObjectAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aDestroyObjectAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlDestroyObjectAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create destroy object action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (token.valueOf("isdestroylinks") == "true")
      a->set_isDestroyLinks(TRUE);
    if (token.valueOf("isdestroyownedobjects") == "true")
      a->set_isDestroyOwnedObjects(TRUE);
        
    a->import_it(in, token);
  }
}

void UmlTestIdentityAction::init()
{
  declareFct("node", "uml:TestIdentityAction", &importIt);
  declareFct("containednode", "uml:TestIdentityAction", &importIt);
}

void UmlTestIdentityAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aTestIdentityAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlTestIdentityAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create test identity action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
        
    a->import_it(in, token);
  }
}

void UmlRaiseExceptionAction::init()
{
  declareFct("node", "uml:RaiseExceptionAction", &importIt);
  declareFct("containednode", "uml:RaiseExceptionAction", &importIt);
}

void UmlRaiseExceptionAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aRaiseExceptionAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlRaiseExceptionAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create raise exception action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
        
    a->import_it(in, token);
  }
}

void UmlReduceAction::init()
{
  declareFct("node", "uml:ReduceAction", &importIt);
  declareFct("containednode", "uml:ReduceAction", &importIt);
}

void UmlReduceAction::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aReduceAction, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlReduceAction * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create reduce action '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (token.valueOf("isordered") == "true")
      a->set_isOrdered(TRUE);
    
    s = token.valueOf("reducer");
        
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	if (token.what() == "reducer") {
	  s = token.xmiIdref();
	  if (! token.closed())
	    in.finish(token.what());
	}
	else
	  a->import(in, token);
      }
    }
    
    if (!s.isEmpty()) {
      QMap<QCString, UmlItem *>::Iterator it = All.find(s);
      
      if (it == All.end())
	Unresolved::addRef(a, s);
      else {
	switch ((*it)->kind()) {
	case anActivity:
	case aState:
	  a->set_Reducer(*it);
	  break;
	default:
	  break;
	}
      }
    }
  }
}

void UmlReduceAction::solve(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
      
  if (it != All.end()) {
    switch ((*it)->kind()) {
    case anActivity:
    case aState:
      set_Reducer(*it);
      break;
    default:
      break;
    }
  }
}

