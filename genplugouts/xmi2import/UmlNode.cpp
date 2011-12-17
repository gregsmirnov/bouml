
#include "UmlNode.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlDeploymentView.h"
void UmlNode::init()
{
  declareFct("ownedmember", "uml:Node", &importIt);
  declareFct("packagedelement", "uml:Node", &importIt);
  declareFct("ownedmember", "uml:Device", &importIt);
  declareFct("packagedelement", "uml:Device", &importIt);

}

void UmlNode::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(aNode, token, in);
  
  if (where == 0)
    return;
    
  QCString s = token.valueOf("name");
  
  if (s.isEmpty()) {
    static unsigned n = 0;
    
    s.sprintf("anonymous_node_%u", ++n);
  }
    
  UmlNode * node = create((UmlDeploymentView *) where, s);
  
  if (node == 0)
    in.error("cannot create node '" + s +
	     "' in '" + where->name() + "'");
  
  node->addItem(token.xmiId(), in);
  
  if (token.xmiType() == "uml:Device")
    node->set_Stereotype("device");

  if (! token.closed()) {
    QCString k = token.what();
    const char * kstr = k;
    
    while (in.read(), !token.close(kstr))
      node->UmlItem::import(in, token);
  }

  node->unload(TRUE, FALSE);
}

int UmlNode::NumberOf;

