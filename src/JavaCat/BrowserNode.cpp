// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#include "BrowserNode.h"
#include "UmlClass.h"
#include "Class.h"
#include "aRelationKind.h"
#include "aDirection.h"
#include "aVisibility.h"
#include "anItemKind.h"
#include "BrowserView.h"

BrowserNode::BrowserNode(BrowserView * parent, const char * n)
    : TreeItem(parent, n) {
}

BrowserNode::BrowserNode(BrowserNode * parent, const char * n)
    : TreeItem(parent, n) {
  // move it at end
  TreeItem * child = parent->firstChild();
  
  while (child->nextSibling())
    child = child->nextSibling();
  
  if (child != this)
    moveItem(child);
}

void BrowserNode::activated() {
}

void BrowserNode::selected() {
}

//

void BrowserNodeList::search(BrowserNode * bn, int k,
			     const QString & s, bool cs)
{
  TreeItem * child;
    
  for (child = bn->firstChild(); child != 0; child = child->nextSibling()) {
    if ((k == aPackage)
	? (((BrowserNode *) child)->isa_package() && 
	   (child->text(0).find(s, 0, cs) != -1))
	: (!((BrowserNode *) child)->isa_package() && 
	   ((k == aClass)
	    ? (child->text(0).find(s, 0, cs) != -1)
	    : (((Class *) child)->get_description().find(s, 0, cs) != -1))))
      append((BrowserNode *) child);
    
    search((BrowserNode *) child, k, s, cs);
  }
}

int BrowserNodeList::compareItems(QCollection::Item item1, QCollection::Item item2)
{
  return ((BrowserNode *) item1)->text(0)
    .compare(((BrowserNode *) item2)->text(0));
}
