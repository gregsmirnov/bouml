
#include "Trigger.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

void Trigger::init()
{
  UmlItem::declareFct("ownedmember", "uml:Trigger", &importIt);
  UmlItem::declareFct("packagedelement", "uml:Trigger", &importIt);
  UmlItem::declareFct("trigger", "uml:Trigger", &importIt);

  UmlItem::declareFct("ownedtrigger", "uml:AnyTrigger", &importIt); // Borland Together 2006 for Eclipse
}

void Trigger::importIt(FileIn & in, Token & token, UmlItem *)
{
  All.insert(token.xmiId(), token.valueOf("name"));
      
  if (! token.closed())
    in.finish(token.what());
}

QCString Trigger::get(QCString idref)
{
  QMap<QCString, QCString>::Iterator iter = All.find(idref);
  static QCString null_str;
  
  return (iter == All.end()) ? null_str : *iter;
}

void Trigger::add(FileIn & in, Token & token, QCString & name, QCString & idref)
{
  // token is <trigger ...>
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

QMap<QCString, QCString> Trigger::All;

