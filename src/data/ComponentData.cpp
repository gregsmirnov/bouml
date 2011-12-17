#error not used
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





#include <qcursor.h>

#include "ComponentData.h"
#include "BrowserComponent.h"
#include "BrowserComponentDiagram.h"
#include "ComponentDialog.h"
#include "GenerationSettings.h"
#include "myio.h"
#include "ToolCom.h"

ComponentData::ComponentData() : associated(0) {
}

ComponentData::~ComponentData() {
#if 0
  if (associated != 0)
    delete associated;
#endif
}

void ComponentData::edit() {
  setName(browser_node->get_name());
  
  (new ComponentDialog(this, QCursor::pos()))->show();
}

//

void ComponentData::send_uml_def(ToolCom * com, BrowserNode * bn, 
				 const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
#if 0
  if (associated == 0)
    com->write_unsigned(0);
  else {    
    QPtrDictIterator<BrowserComponent> itd(*associated);
    unsigned n = 0;
    
    while (itd.current()) {
      if (! itd.current()->deletedp())
	n += 1;
      ++itd;
    }
    
    com->write_unsigned(n);
    
    itd.toFirst();
    
    while (itd.current()) {
      if (! itd.current()->deletedp())
	itd.current()->write_id(com);
      ++itd;
    }
  }
#endif
}

bool ComponentData::tool_cmd(ToolCom * com, const char * args,
			     BrowserNode * bn,
			     const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
#if 0
      case addAssocComponentCmd:
	{
	  BrowserComponent * cp = (BrowserComponent *) com->get_id(args);
	  
	  associate(cp);
	}
	break;
      case removeAssocComponentCmd:
	{
	  BrowserComponent * cp = (BrowserComponent *) com->get_id(args);
	  
	  unassociate(cp);
	}
	break;
      case removeAllAssocComponentsCmd:
	if (associated != 0) {
	  QPtrDictIterator<BrowserComponent> it(*associated);
	  
	  while (it.current()) {
	    disconnect(it.current()->get_data(), SIGNAL(deleted()),
		       this, SLOT(on_delete()));
	    ++it;
	  }
	  
	  delete associated;
	  associated = 0;
	}
	break;
#endif
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
      
      // ok case
      bn->modified();
      bn->package_modified();
      modified();
      com->write_ack(TRUE);
    }
  }
  else
    return BasicData::tool_cmd(com, args, bn, comment);
  
  return TRUE;
}
      
void ComponentData::on_delete() {
#if 0
  if (associated != 0) {

    bool modp = FALSE;
    QPtrDictIterator<BrowserComponent> it(*associated);

    while (it.current()) {
      if (it.current()->deletedp()) {
	modp = TRUE;
	associated->remove(it.current()); // update it
      }
      else
	++it;
    }
    if (modp)
      modified();
  }
#endif
  browser_node->on_delete();
}
  
void ComponentData::associate(BrowserComponent * other) {
#if 0
  if ((associated == 0) || (associated->find(other) == 0)) {
    connect(other->get_data(), SIGNAL(deleted()), this, SLOT(on_delete()));
    if (associated == 0)
      associated = new QPtrDict<BrowserComponent>;
    
    associated->insert(other, other);
    browser_node->modified();
    browser_node->package_modified();
    modified();
  }
#endif
}

void ComponentData::unassociate(BrowserComponent * other) {
#if 0
  disconnect(other->get_data(), SIGNAL(deleted()), this, SLOT(on_delete()));
  associated->remove(other);
  browser_node->modified();
  browser_node->package_modified();
  modified();
#endif
}

void ComponentData::update_associated(QPtrDict<BrowserComponent> & d) {
#if 0
  if (associated != 0) {
    QPtrDictIterator<BrowserComponent> it(*associated);

    while (it.current()) {
      if (d.find(it.current()) == 0) {
	disconnect(it.current()->get_data(), SIGNAL(deleted()),
		   this, SLOT(on_delete()));
	associated->remove(it.current());
      }
      else
	++it;
    }
  }
  else
    associated = new QPtrDict<BrowserComponent>((d.count() >> 4) + 1);
  
  QPtrDictIterator<BrowserComponent> it(d);
  
  while (it.current()) {
    if (associated->find(it.current()) == 0) {
      associated->insert(it.current(), it.current());
      if (associated->size() < (associated->count() >> 4))
	associated->resize((associated->count() >> 4) + 1);
      connect(it.current()->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete()));
    }
    ++it;
  }
#endif
}

void ComponentData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  
#if 0
  if (associated != 0) {
    nl_indent(st);
    st << "associated_components";
    indent(+1);
    
    QPtrDictIterator<BrowserComponent> it(*associated);

    while (it.current()) {
      if (it.current()->deletedp())
	warning += QString("<p><b>") + browser_node->get_name() + 
	  "</b>'s associated component <b>" + it.current()->get_name() +
	    "</b> is deleted\n";
      else {
	nl_indent(st);
	it.current()->save(st, TRUE, warning);
      }
      ++it;
    }
    
    indent(-1);
    nl_indent(st);
    st << "end";
  }
#endif
}

void ComponentData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  
#if 0
  if (!strcmp(k, "associated_components")) {
    associated = new QPtrDict<BrowserComponent>();
    
    while (strcmp(k = read_keyword(st), "end")) {
      BrowserComponent * c =
	BrowserComponent::read_ref(st, k);	// k not updated
      
      associated->insert(c, c);
      connect(c->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete()));
    }
    
    if (associated->size() < (associated->count() >> 4))
      associated->resize((associated->count() >> 4) + 1);
    
    k = read_keyword(st);
  }
#endif
}
