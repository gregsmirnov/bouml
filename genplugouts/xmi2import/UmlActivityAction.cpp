
#include "UmlActivityAction.h"
#include "UmlItem.h"
#include "Token.h"
#include "FileIn.h"

UmlItem * UmlActivityAction::container(anItemKind kind, Token & token, FileIn & in) {
  switch (kind) {
  case anActivityPin:
    return this;
  default:
    return parent()->container(kind, token, in);
  }
}

void UmlActivityAction::import_it(FileIn & in, Token & token) {
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr))
      import(in, token);
  }
}

void UmlActivityAction::import(FileIn & in, Token & token) {
  QCString s = token.what();
  
  if ((s == "precondition") || (s == "postcondition") ||
      (s == "localprecondition") || (s == "localpostcondition"))
    readCondition(in, token);
  else if (s == "ownedrule")
    set_Constraint(UmlItem::readConstraint(in, token));
  else
    UmlItem::import(in, token);

}

void UmlActivityAction::readCondition(FileIn & in, Token & token) {
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
      
    while (in.read(), !token.close(kstr)) {
      QCString s = token.what();
      
      if (s == "specification") {
	QCString v = token.valueOf("body");
	
	if (v.isNull())
	  v = token.valueOf("value");	// UMODEL
	
	if (! token.closed()) {
	  while (in.read(), !token.close("specification")) {
	    QCString b = token.what();
	    
	    if (b == "body")
	      v = in.body("body");
	    else if (! token.closed())
	      in.finish(b);
	  }
	}
	
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

