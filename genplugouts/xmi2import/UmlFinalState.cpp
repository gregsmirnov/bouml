
#include "UmlFinalState.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

void UmlFinalState::init()
{
  declareFct("subvertex", "uml:FinalState", &importIt);

}

void UmlFinalState::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aFinalState, token, in);
    
  if (where != 0) {
    UmlFinalState * fs = UmlFinalState::create(where);
    
    if (fs == 0)
      in.error("cannot create final state in '" + where->name() + "'");

    fs->addItem(token.xmiId(), in);
    
    if (! token.closed()) {
      QCString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr))
	fs->UmlItem::import(in, token);
    }
  }
}

