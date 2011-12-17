
#include "UmlActivityObject.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlActivityPin.h"
#include "UmlCom.h"
void UmlActivityObject::init()
{
  declareFct("node", "uml:ObjectNode", &importIt);
  declareFct("containednode", "uml:ObjectNode", &importIt);
  
  declareFct("node", "uml:DataStoreNode", &importIt);
  declareFct("containednode", "uml:DataStoreNode", &importIt);
  
  declareFct("node", "uml:CentralBufferNode", &importIt);
  declareFct("containednode", "uml:CentralBufferNode", &importIt);

  UmlActivityPin::init();
}

void UmlActivityObject::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anActivityObject, token, in);
    
  if (where != 0) {
    QCString s = token.valueOf("name");
    UmlActivityObject * a = create(where, s);
    
    if (a == 0)
      in.error("cannot create activity object '"
	       + s + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    QCString ste;
    
    s = token.xmiType();
    switch (((const char *) s)[0]) {
    case 'D':
      ste = "datastore";
      break;
    case 'C':
      ste = "centralBuffer";
      break;
    default:
      break;
    }
    
    a->import_it(in, token);
    
    if (! ste.isNull())
      a->set_Stereotype(ste);
  }

}

void UmlActivityObject::setType(QCString idref) {
  UmlTypeSpec ts;
  
  if (UmlItem::setType(idref, 0, ts))
    set_Type(ts);

}

void UmlActivityObject::setType(Token & token) {
  UmlTypeSpec ts;
  
  if (UmlItem::setType(token, 0, ts))
    set_Type(ts);

}

void UmlActivityObject::setOrdering(QCString s, FileIn & in) {
 if (s == "unordered")
   set_Ordering(unordered);
 else if (s == "ordered")
   set_Ordering(ordered);
 else if (s == "LIFO")
   set_Ordering(lifo);
 else if (s == "FIFO")
   set_Ordering(fifo);
 else if (! s.isEmpty())
   in.warning("wrong ordering '" + s + "'");
}

void UmlActivityObject::setSelection(QCString idref) {
  QMap<QCString, QCString>::Iterator it = OpaqueDefs.find(idref);
  
  if (it != OpaqueDefs.end())
    set_Selection(*it);
  else
    UnresolvedWithContext::add(this, idref, 2);
}

void UmlActivityObject::setInState(QCString s) {
  if (FromBouml && (s.left(8) != "BOUML_0x"))
    set_InState(s);
  else {
    QMap<QCString, UmlItem *>::Iterator it = All.find(s);
  
    if (it != All.end()) {
      if ((*it)->kind() == aState)
	set_InState((*it)->name());
    }
    else
      UnresolvedWithContext::add(this, s, 1);
  }
}

void UmlActivityObject::importMultiplicity(FileIn & in, Token & token, bool upper) {
  QCString s = token.valueOf("value");
  
  if (!s.isEmpty() && 
      (s != "Unspecified")) {	// VP
    QCString m = multiplicity();
    
    if (m.isEmpty())
      m = s;
    else if (m != s) {
      if (upper)
	m += ".." + s;
      else
	m = s + ".." + m;
    }
    
    set_Multiplicity(m);
  }

  if (! token.closed())
    in.finish(token.what());
}

void UmlActivityObject::import_it(FileIn & in, Token & token) {
  if (token.valueOf("iscontroltype") == "true")
    set_IsControlType(TRUE);
  
  QCString s;
    
  if (!(s = token.valueOf("ordering")).isEmpty())
    setOrdering(s, in);
  if (!(s = token.valueOf("selection")).isEmpty())
    setSelection(s);
  if (!(s = token.valueOf("instate")).isEmpty())
    setInState(s);
  if (!(s = token.valueOf("type")).isEmpty())
    setType(s);
  
  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr)) {
      s = token.what();
      
      if (s == "selection") {
	setSelection(token.xmiIdref());
	if (! token.closed())
	  in.finish(s);
      }
      else if (s == "instate") {
	setInState(token.xmiIdref());
	if (! token.closed())
	  in.finish(s);
      }
      else if (s == "type") {
	setType(token);
	if (! token.closed())
	  in.finish(s);
      }
      else if (s == "lowervalue")
	importMultiplicity(in, token, FALSE);
      else if (s == "uppervalue")
	importMultiplicity(in, token, TRUE);
      else if (s == "upperbound") {
	if (! token.closed())
	  in.finish(s);
      }
      else
	UmlItem::import(in, token);
    }
  }
}

void UmlActivityObject::solve(int context, QCString idref) {
  switch (context) {
  case 0:
    // type
    {
      UmlTypeSpec ts;
      
      if (getType(idref, ts))
	set_Type(ts);
      else if (!FileIn::isBypassedId(idref))
	UmlCom::trace("activity object : unknown type reference '" + idref + "'<br>");
    }
    break;
  case 1:
    // state
    {
      QMap<QCString, UmlItem *>::Iterator it = All.find(idref);
      
      if (it != All.end()) {
	if ((*it)->kind() == aState)
	  set_InState((*it)->name());
      }
      else if (!FileIn::isBypassedId(idref))
	UmlCom::trace("activity object : unknown state reference '" + idref + "'<br>");
    }
    break;
  default:
    // selection
    {
      QMap<QCString, QCString>::Iterator it = OpaqueDefs.find(idref);
      
      if (it != OpaqueDefs.end())
	set_Selection(*it);
      else if (!FileIn::isBypassedId(idref))
	UmlCom::trace("activity object : unknown opaque definition reference '" + idref + "'<br>");
    }
    break;
  }
}

