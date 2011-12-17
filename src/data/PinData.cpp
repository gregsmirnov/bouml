// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
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

#include "PinData.h"
#include "PinDialog.h"
#include "BrowserClass.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

PinData::PinData() {
}

PinData::PinData(const PinParamData & pd) {
  *((PinParamData *) this) = pd;
}

PinData::PinData(PinData * model, BrowserNode * bn)
    : SimpleData(model), PinParamData(model) {
  browser_node = bn;
}

void PinData::edit() {
  setName(browser_node->get_name());
    
  (new PinDialog(this))->show();
}

void PinData::do_connect(BrowserClass * c) {
  connect(c->get_data(), SIGNAL(deleted()), this, SLOT(on_delete()));
}

void PinData::do_disconnect(BrowserClass * c) {
  disconnect(c->get_data(), 0, this, 0);
}

void PinData::on_delete() {
  BrowserClass * cl = get_type().type;
  
  if ((cl != 0) && cl->deletedp())
    set_type((BrowserClass *) 0);
}

void PinData::send_uml_def(ToolCom * com, BrowserNode * bn,
			   const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
  PinParamData::send_uml_def(com);
}

void PinData::send_cpp_def(ToolCom * com) {
  PinParamData::send_cpp_def(com);
}

void PinData::send_java_def(ToolCom * com) {
  PinParamData::send_java_def(com);
}

bool PinData::tool_cmd(ToolCom * com, const char * args,
		       BrowserNode * bn, const QString & comment) {
  BooL ack;

  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      ack = FALSE;
    else if (! PinParamData::tool_cmd(com, args, ack))
      return BasicData::tool_cmd(com, args, bn, comment);
  }
  else if (! PinParamData::tool_cmd(com, args, ack))
    return BasicData::tool_cmd(com, args, bn, comment);
  
  if (ack) {
    // ok case
    bn->package_modified();
    modified();
  }
  
  com->write_ack(ack);
  return TRUE;
}

void PinData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  PinParamData::save(st, warning);
}

void PinData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  PinParamData::read(st, k);	// updates k
}
