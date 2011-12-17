
#include "UmlInterruptibleActivityRegion.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

UmlItem * UmlInterruptibleActivityRegion::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case anExpansionRegion:
  case anInterruptibleActivityRegion:
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

void UmlInterruptibleActivityRegion::init()
{
  declareFct("group", "uml:InterruptibleActivityRegion", &importIt);

}

void UmlInterruptibleActivityRegion::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anInterruptibleActivityRegion, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlInterruptibleActivityRegion * r = create(where, s);
    
    if (r == 0)
      in.error("cannot create interruptible region '"
	       + s + "' in '" + where->name() + "'");
    
    r->addItem(token.xmiId(), in);
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr))
	r->UmlItem::import(in, token);
    }
  }
}

