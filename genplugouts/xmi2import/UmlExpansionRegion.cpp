
#include "UmlExpansionRegion.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

#include "UmlExpansionNode.h"
UmlItem * UmlExpansionRegion::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case anExpansionRegion:
  case anInterruptibleActivityRegion:
  case anExpansionNode:
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

void UmlExpansionRegion::init()
{
  declareFct("group", "uml:ExpansionRegion", &importIt);
  declareFct("node", "uml:ExpansionRegion", &importIt);
  
  UmlExpansionNode::init();
}

void UmlExpansionRegion::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anExpansionRegion, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlExpansionRegion * r = create(where, s);
    
    if (r == 0)
      in.error("cannot create expansion region '"
	       + s + "' in '" + where->name() + "'");
    
    r->addItem(token.xmiId(), in);
    
    if (token.valueOf("mustisolate") == "true")
      r->set_isMustIsolate(TRUE);
    
    QCString v = token.valueOf("mode").lower();
    
    if (v == "parallel")
      r->set_Mode(parallelExecution);
    else if (v == "iterative")
      r->set_Mode(iterativeExecution);
    else if (v == "stream")
      r->set_Mode(streamExecution);
    else if (! v.isEmpty())
      in.error("illegal mode '" + v + "'");
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr))
	r->UmlItem::import(in, token);
    }
  }
}

