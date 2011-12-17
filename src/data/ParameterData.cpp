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

#include "ParameterData.h"
#include "ParameterDialog.h"
#include "BrowserClass.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

ParameterData::ParameterData() {
}

ParameterData::ParameterData(ParameterData * model, BrowserNode * bn)
    : SimpleData(model), PinParamData(model) {
  default_value = model->default_value;
  browser_node = bn;
}

void ParameterData::edit() {
  setName(browser_node->get_name());
    
  (new ParameterDialog(this))->show();
}

void ParameterData::do_connect(BrowserClass * c) {
  connect(c->get_data(), SIGNAL(deleted()), this, SLOT(on_delete()));
}

void ParameterData::do_disconnect(BrowserClass * c) {
  disconnect(c->get_data(), 0, this, 0);
}

void ParameterData::on_delete() {
  BrowserClass * cl = get_type().type;
  
  if ((cl != 0) && cl->deletedp())
    set_type((BrowserClass *) 0);
}

void ParameterData::send_uml_def(ToolCom * com, BrowserNode * bn,
				 const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
  PinParamData::send_uml_def(com);
  com->write_string(default_value);
}

void ParameterData::send_cpp_def(ToolCom * com) {
  PinParamData::send_cpp_def(com);
}

void ParameterData::send_java_def(ToolCom * com) {
  PinParamData::send_java_def(com);
}

bool ParameterData::tool_cmd(ToolCom * com, const char * args,
			     BrowserNode * bn, const QString & comment) {
  BooL ack;

  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      ack = FALSE;
    else {
      switch ((unsigned char) args[-1]) {
      case setDefaultValueCmd:
	default_value = args;
	ack = TRUE;
	break;
      default:
        if (! PinParamData::tool_cmd(com, args, ack))
	  return BasicData::tool_cmd(com, args, bn, comment);
      }
    }
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

void ParameterData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  PinParamData::save(st, warning);

  if (!default_value.isEmpty()) {
    st << " defaultvalue ";
    save_string(default_value, st);
  }
}

void ParameterData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  PinParamData::read(st, k);	// updates k
    
  if (!strcmp(k, "defaultvalue")) {
    default_value = read_string(st);
    k = read_keyword(st);
  }
}
