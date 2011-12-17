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





#include <stdio.h>

#include "UmlDrag.h"
#include "BrowserNode.h"

const QString UmlDrag::Key = "Bouml/";
QString UmlDrag::postfix;
bool UmlDrag::ro;

UmlDrag::UmlDrag(BrowserNode * bn, QWidget * parent, const char * name)
    : QStoredDrag(UmlDrag::Key + bn->drag_key(), parent, name) {
  // stay in the same application : can use address directly
  QByteArray a(sizeof(bn));
  
  memcpy(a.data(), &bn, sizeof(bn));
  setEncodedData(a);
  
  postfix = bn->drag_postfix();
  ro = ((bn->parent() != 0) &&
	!((BrowserNode *) bn->parent())->is_writable());
}


bool UmlDrag::canDecode(QDragMoveEvent * e, UmlCode type,
			bool withpostfix, bool evenro)
{
  if (ro && ! evenro)
    return FALSE;
  
  return (e->source() != 0) &&
    e->provides((withpostfix) ? UmlDrag::Key + QString::number(type) + postfix
			      : UmlDrag::Key + QString::number(type));
}

bool UmlDrag::canDecode(QDragMoveEvent * e, const QString & type)
{
  return !ro && (e->source() != 0) && e->provides(UmlDrag::Key + type);
}

BrowserNode * UmlDrag::decode(QDropEvent * e, UmlCode type,
			      bool withpostfix)
{
  QByteArray payload =
    e->data((withpostfix) ? UmlDrag::Key + QString::number(type) + postfix
			  : UmlDrag::Key + QString::number(type));
  
  if (payload.size()) {
    e->accept();
    BrowserNode * bn;
    
    memcpy(&bn, payload.data(), sizeof(bn));
    return bn;
  }
  
  return 0;
}

BrowserNode * UmlDrag::decode(QDropEvent * e, const QString & type)
{
  QByteArray payload = e->data(UmlDrag::Key + type);
  
  if (payload.size()) {
    e->accept();
    BrowserNode * bn;
    
    memcpy(&bn, payload.data(), sizeof(bn));
    return bn;
  }
  
  return 0;
}

