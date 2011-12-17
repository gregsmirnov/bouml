
#include "UmlActivity.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

#include "UmlExpansionRegion.h"
#include "UmlInterruptibleActivityRegion.h"
#include "UmlActivityPartition.h"
#include "UmlActivityActionClasses.h"
#include "UmlActivityControlNode.h"
#include "UmlActivityParameter.h"
#include "UmlFlow.h"
#include "UmlCom.h"

UmlItem * UmlActivity::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case anExpansionRegion:
  case anInterruptibleActivityRegion:
  case aPartition:
  case anOpaqueAction:
  case anAcceptEventAction:
  case aReadVariableValueAction:
  case aClearVariableValueAction:
  case aWriteVariableValueAction:
  case anAddVariableValueAction:
  case aRemoveVariableValueAction:
  case aCallBehaviorAction:
  case aCallOperationAction:
  case aSendObjectAction:
  case aSendSignalAction:
  case aBroadcastSignalAction:
  case anUnmarshallAction:
  case aValueSpecificationAction:
  case anAcceptCallAction:
  case aReplyAction:
  case aCreateObjectAction:
  case aDestroyObjectAction:
  case aTestIdentityAction:
  case aRaiseExceptionAction:
  case aReduceAction:
  case anActivityObject:
  case anInitialActivityNode:
  case aFlowFinalActivityNode:
  case anActivityFinalActivityNode:
  case aDecisionActivityNode:
  case aMergeActivityNode:
  case aForkActivityNode:
  case aJoinActivityNode:
    return this;
  default:
    return parent()->container(kind, token, in);
  }

}

void UmlActivity::solve(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("state : unknown operation reference '" + idref + "'<br>");
  }
  else if ((*it)->kind() == anOperation)
    set_Specification((UmlOperation *) *it);
}

void UmlActivity::init()
{
  declareFct("ownedmember", "uml:Activity", &importIt);
  declareFct("packagedelement", "uml:Activity", &importIt);

  UmlExpansionRegion::init();
  UmlInterruptibleActivityRegion::init();
  UmlActivityPartition::init();
  UmlOpaqueAction::init();
  UmlAcceptEventAction::init();
  UmlReadVariableValueAction::init();
  UmlClearVariableValueAction::init();
  UmlWriteVariableValueAction::init();
  UmlAddVariableValueAction::init();
  UmlRemoveVariableValueAction::init();
  UmlCallBehaviorAction::init();
  UmlCallOperationAction::init();
  UmlSendObjectAction::init();
  UmlSendSignalAction::init();
  UmlBroadcastSignalAction::init();
  UmlUnmarshallAction::init();
  UmlValueSpecificationAction::init();
  UmlAcceptCallAction::init();
  UmlReplyAction::init();
  UmlCreateObjectAction::init();
  UmlDestroyObjectAction::init();
  UmlTestIdentityAction::init();
  UmlRaiseExceptionAction::init();
  UmlReduceAction::init();
  UmlActivityControlNode::init();
  UmlFlow::init();
}

void UmlActivity::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anActivity, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    
    if (s.isEmpty()) {
      static unsigned n = 0;
      
      s.sprintf("anonymous_activity_%u", ++n);
    }
    
    UmlActivity * a = create((UmlClassView *) where, s);
    
    if (a == 0)
      in.error("cannot create activity '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (token.valueOf("isreadonly") == "true")
      a->set_isReadOnly(TRUE);
    
    if (token.valueOf("issingleexecution") == "true")
      a->set_isSingleExecution(TRUE);

    if (token.valueOf("isactive") == "true")
      a->set_isActive(TRUE);
    
    QCString spec = token.valueOf("specification");
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr)) {
	s = token.what();
	
	if ((s == "precondition") || (s == "postcondition") ||
	    (s == "localprecondition") || (s == "localpostcondition"))
	  a->readCondition(in, token);
	else if (s == "ownedparameter")
	  a->readParameter(in, token);
	else if ((s == "node") &&
		 (token.xmiType() == "uml:ActivityParameterNode"))
	  a->readParameterNode(in, token);
	else if (s == "specification") {
	  spec = token.xmiIdref();
	  if (! token.closed())
	    in.finish(s);
	}
	else if (s == "ownedrule")
	  a->set_Constraint(UmlItem::readConstraint(in, token));
	else
	  a->UmlItem::import(in, token);
      }
    }
    
    if (! spec.isEmpty()) {
      QMap<QCString, UmlItem *>::Iterator it = All.find(spec);
      
      if (it == All.end())
	Unresolved::addRef(a, spec);
      else if ((*it)->kind() == anOperation)
	a->set_Specification((UmlOperation *) *it);
    }

    a->unload(TRUE, FALSE);
  }
}

void UmlActivity::readCondition(FileIn & in, Token & token) {
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
      
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if (s == "specification") {
	QCString v = token.valueOf("body");
	
	if (v.isNull())
	  v = token.valueOf("value");	// UMODEL
	
	if (! v.isEmpty()) {
	  if (k[1] == 'r')
	    set_PreCondition(v);
	  else
	    set_PostCondition(v);
	}
      }
      
      if (! token.closed())
	in.finish(s);
    }
  }
}

void UmlActivity::readParameter(FileIn & in, Token & token) {
  // the parameter may already exist because of a 
  // ActivityParameterNode definition, search for it
  UmlActivityParameter * param = 0;
  QCString s = token.valueOf("name");
  const QVector<UmlItem> ch = children();
  unsigned int n = ch.size();
  int i;
  
  for (i = 0; i != (int) n; i += 1) {
    UmlActivityParameter * p = dynamic_cast<UmlActivityParameter *>(ch[i]);
    
    if ((p != 0) && (p->name() == s)) {
      param = p;
      break;
    }
  }
	
  if (param == 0) {
    param = UmlActivityParameter::create(this, s);
    
    if (param == 0)
      in.error("cannot create activity parameter '"
	       + s + "' in '" + name() + "'");

    param->addItem(token.xmiId(), in);
  }

  param->readParameter(in, token);
}

void UmlActivity::readParameterNode(FileIn & in, Token & token) {
  // the parameter node may already exist because of a 
  // ActivityParameter definition, search for it
  UmlActivityParameter * param = 0;
  QCString s = token.valueOf("name");
  const QVector<UmlItem> ch = children();
  unsigned int n = ch.size();
  int i;
  
  for (i = 0; i != (int) n; i += 1) {
    UmlActivityParameter * p = dynamic_cast<UmlActivityParameter *>(ch[i]);
    
    if ((p != 0) && (p->name() == s)) {
      param = p;
      break;
    }
  }
	
  if (param == 0) {
    param = UmlActivityParameter::create(this, s);
    
    if (param == 0)
      in.error("cannot create activity parameter node '"
	       + s + "' in '" + name() + "'");

    param->addItem(token.xmiId(), in);
  }

  param->readParameterNode(in, token);

}

int UmlActivity::NumberOf;

