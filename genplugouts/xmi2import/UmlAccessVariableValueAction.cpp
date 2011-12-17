
#include "UmlAccessVariableValueAction.h"
#include "FileIn.h"
#include "Token.h"

#include "UmlCom.h"
void UmlAccessVariableValueAction::import_it(FileIn & in, Token & token) {
  QCString s = token.valueOf("variable");
  
  if (! s.isEmpty())
    setVariable(s);
    
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "variable") {
	setVariable(token.xmiIdref());
	if (! token.closed())
	  in.finish("variable");
      }
      else
	import(in, token);
    }
  }
}

void UmlAccessVariableValueAction::setVariable(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end())
    Unresolved::addRef(this, idref);
  else {
    switch ((*it)->kind()) {
    case anAttribute:
    case aRelation:
      set_Variable(*it);
    default:
      break;
    }
  }
}

void UmlAccessVariableValueAction::solve(QCString idref) {
  QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
  
  if (it == All.end()) {
    if (!FileIn::isBypassedId(idref))
      UmlCom::trace("activity action : unknown variable reference '" + idref + "'<br>");
  }
  else {
    switch ((*it)->kind()) {
    case anAttribute:
    case aRelation:
      set_Variable(*it);
    default:
      break;
    }
  }
}

