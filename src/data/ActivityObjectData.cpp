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

#include "ActivityObjectData.h"
#include "ActivityObjectDialog.h"
#include "BrowserClass.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

ActivityObjectData::ActivityObjectData() {
}

ActivityObjectData::ActivityObjectData(ActivityObjectData * model, BrowserNode * bn)
    : SimpleData(model), ObjectData(model) {
  browser_node = bn;
}

void ActivityObjectData::edit(const char * what, QStringList & st) {
  setName(browser_node->get_name());
    
  (new ActivityObjectDialog(this, what, st))->show();
}

void ActivityObjectData::send_uml_def(ToolCom * com, BrowserNode * bn,
				      const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);  
  ObjectData::send_uml_def(com);
}

void ActivityObjectData::send_cpp_def(ToolCom * com) {
  ObjectData::send_cpp_def(com);
}

void ActivityObjectData::send_java_def(ToolCom * com) {
  ObjectData::send_java_def(com);
}

void ActivityObjectData::do_connect(BrowserClass * c) {
  connect(c->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(c->get_data(), SIGNAL(deleted()), this, SLOT(on_delete()));
}

void ActivityObjectData::do_disconnect(BrowserClass * c) {
  disconnect(c->get_data(), 0, this, 0);
}

void ActivityObjectData::on_delete() {
  BrowserClass * cl = get_type().type;
  
  if ((cl != 0) && cl->deletedp())
    set_type((BrowserClass *) 0);
}

bool ActivityObjectData::tool_cmd(ToolCom * com, const char * args,
				  BrowserNode * bn,
				  const QString & comment) {
  if ((unsigned char) args[-1] >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else if (! ObjectData::tool_cmd(com, args))
      return BasicData::tool_cmd(com, args, bn, comment);
  }
  else if (! ObjectData::tool_cmd(com, args))
    return BasicData::tool_cmd(com, args, bn, comment);
      
  // ok case
  bn->modified();
  bn->package_modified();
  modified();
  com->write_ack(TRUE);
  return TRUE;
}

void ActivityObjectData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  nl_indent(st);
  ObjectData::save(st, warning);
}

void ActivityObjectData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  ObjectData::read(st, k);	// updates k
}
