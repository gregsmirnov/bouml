
#include "UmlRegion.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

UmlItem * UmlRegion::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case aState:
  case aRegion:
  case anInitialPseudoState:
  case anEntryPointPseudoState:
  case aFinalState:
  case aTerminatePseudoState:
  case anExitPointPseudoState:
  case aDeepHistoryPseudoState:
  case aShallowHistoryPseudoState:
  case aJunctionPseudoState:
  case aChoicePseudoState:
  case aForkPseudoState:
  case aJoinPseudoState:
    return this;
  default:
    return parent()->container(kind, token, in);
  }

}

void UmlRegion::init()
{
  declareFct("region", "uml:Region", &importIt);
  declareFct("region", "", &importIt);	// sometimes with Visual Paradigm
}

void UmlRegion::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aRegion, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    
    UmlRegion * st = create((UmlState *) where, s);
    
    if (st == 0)
      in.error("cannot create region '" + s +
	       "' in '" + where->name() + "'");
    
    st->addItem(token.xmiId(), in);
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr))
	st->UmlItem::import(in, token);
    }
  }
}

