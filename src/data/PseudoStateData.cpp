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

#include "PseudoStateData.h"
#include "BrowserClass.h"
#include "ClassData.h"
#include "BrowserPseudoState.h"
#include "PseudoStateDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"
#include "strutil.h"

PseudoStateData::PseudoStateData() : reference(0) {
}

// doesn't copy associated classes or artifacts
PseudoStateData::PseudoStateData(PseudoStateData * model, BrowserNode * bn)
    : SimpleData(model) {
  browser_node = bn;
  reference = model->reference;
}

PseudoStateData::~PseudoStateData() {
}

void PseudoStateData::edit() {
  setName(browser_node->get_name());
  
  (new PseudoStateDialog(this))->show();
}

void PseudoStateData::set_reference(BrowserPseudoState * st) {
  if (reference != 0)
    disconnect(reference->get_data(), SIGNAL(deleted()),
	       this, SLOT(on_delete()));
  if ((reference = st) != 0) {
    connect(reference->get_data(), SIGNAL(deleted()),
	    this, SLOT(on_delete()));
  }
}

void PseudoStateData::on_delete() {
  if ((reference != 0) && reference->deletedp()) {
    disconnect(reference->get_data(), SIGNAL(deleted()),
	       this, SLOT(on_delete()));
    reference = 0;
  }
}

//

void PseudoStateData::send_uml_def(ToolCom * com, BrowserNode * bn, 
			       const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
  
  switch (browser_node->get_type()) {
  case EntryPointPS:
  case ExitPointPS:
    if (com->api_format() >= 55){
      if ((reference == 0) ||
	  !((BrowserPseudoState *) browser_node)->can_reference(reference))
	com->write_id(0);
      else
	reference->write_id(com);
    }
    break;
  default:
    break;
  }
}

bool PseudoStateData::tool_cmd(ToolCom * com, const char * args,
			       BrowserNode * bn, const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setDerivedCmd:
	{
	  BrowserPseudoState * st = (BrowserPseudoState *) com->get_id(args);

	  if (st == reference) {
	    com->write_ack(TRUE);
	    return TRUE;
	  }
	  if ((st != 0) &&
	      !((BrowserPseudoState *) browser_node)->can_reference(st)) {
	    com->write_ack(FALSE);
	    return TRUE;
	  }
	  set_reference(st);
	}
	break;
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
      
      // ok case
      bn->modified();
      modified();
      com->write_ack(TRUE);
    }
  }
  else
    return BasicData::tool_cmd(com, args, bn, comment);
  
  return TRUE;
}
      
//

void PseudoStateData::save(QTextStream & st, QString & warning) const {
  if ((reference != 0) &&
      ((BrowserPseudoState *) browser_node)->can_reference(reference)) {
    nl_indent(st);
    st << "reference ";
    reference->save(st, TRUE, warning);
  }
  
  BasicData::save(st, warning);
}

void PseudoStateData::read(char * & st, char * & k) {
  if (!strcmp(k, "reference")) {
    set_reference(BrowserPseudoState::read_ref(st));
    k = read_keyword(st);
  }
  
  BasicData::read(st, k);	// updates k
}
