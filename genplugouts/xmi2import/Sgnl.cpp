
#include "Sgnl.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

void Signal::init()
{
  UmlItem::declareFct("ownedmember", "uml:Signal", &importIt);
  UmlItem::declareFct("packagedelement", "uml:Signal", &importIt);
  UmlItem::declareFct("signal", "uml:Signal", &importIt);

}

void Signal::importIt(FileIn & in, Token & token, UmlItem *)
{
  All.insert(token.xmiId(), token.valueOf("name"));
      
  if (! token.closed())
    in.finish(token.what());
}

QCString Signal::get(QCString idref)
{
  QMap<QCString, QCString>::Iterator iter = All.find(idref);
  static QCString null_str;
  
  return (iter == All.end()) ? null_str : *iter;
}

void Signal::add(FileIn & in, Token & token, QCString & name, QCString & idref)
{
  // token is <signal ...>
  QCString t = token.xmiIdref();
  
  if (! t.isEmpty()) {
    QMap<QCString, QCString>::Iterator iter = All.find(t);
    
    if (iter == All.end()) {
      idref = t;
      name = "";
    }
    else {
      name = *iter;
      idref = "";
    }
  }
  else {
    name = token.valueOf("name");
    idref = "";
    All.insert(token.xmiId(), name);
  }

  if (! token.closed())
    in.finish(token.what());

}

QMap<QCString, QCString> Signal::All;

