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

#include "ParameterSetData.h"
#include "ParameterSetDialog.h"
#include "BrowserPin.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

ParameterSetData::ParameterSetData() {
}

ParameterSetData::ParameterSetData(ParameterSetData * model, BrowserNode * bn)
    : SimpleData(model) {
  browser_node = bn;
  set_pins(model->pins);
}

void ParameterSetData::set_pins(const QValueList<BrowserPin *> & l) {
  QValueList<BrowserPin *>::Iterator iter;

  // disconnect old pins
  for (iter = pins.begin(); iter != pins.end(); iter++)
    disconnect((*iter)->get_data(), SIGNAL(deleted()), this, SLOT(check()));

  pins = l;

  // connect new pins
  for (iter = pins.begin(); iter != pins.end(); iter++)
    connect((*iter)->get_data(), SIGNAL(deleted()), this, SLOT(check()));
}

void ParameterSetData::check() {
  // remove deleted items, note : can't disconnect
  QValueList<BrowserPin *>::Iterator iter = pins.begin();

  while (iter != pins.end())
    if ((*iter)->deletedp())
      iter = pins.remove(iter);
    else
      iter++;
}

void ParameterSetData::edit() {
  setName(browser_node->get_name());
    
  (new ParameterSetDialog(this))->show();
}

void ParameterSetData::send_uml_def(ToolCom * com, BrowserNode * bn,
				    const QString & comment) {
  SimpleData::send_uml_def(com, bn, comment);
  com->write_unsigned(pins.count());

  QValueList<BrowserPin *>::ConstIterator iter = pins.begin();

  for (iter = pins.begin(); iter != pins.end(); iter++)
    (*iter)->write_id(com);
}

bool ParameterSetData::tool_cmd(ToolCom * com, const char * args,
				BrowserNode * bn,
				const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case replaceParameterCmd:
	// replace all the pins
	{
	  unsigned n = com->get_unsigned(args);
	  QValueList<BrowserPin *> l;

	  while (n--)
	    l.append((BrowserPin *) com->get_id(args));
	  set_pins(l);
	}
	break;
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

void ParameterSetData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  nl_indent(st);
  st << "pins";
  indent(+1);

  QValueList<BrowserPin *>::ConstIterator iter = pins.begin();

  for (iter = pins.begin(); iter != pins.end(); iter++) {
    nl_indent(st);
    (*iter)->save(st, TRUE, warning);
  }

  indent(-1);
  nl_indent(st);
  st << "end";
}

void ParameterSetData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k

  if (strcmp(k, "pins"))
    wrong_keyword(k, "pins");
  
  QValueList<BrowserPin *> l;

  while (strcmp((k = read_keyword(st)), "end")) {
    BrowserPin * pa = BrowserPin::read(st, k, 0);

    if (pa != 0)
      l.append(pa);
  }

  set_pins(l);

  k = read_keyword(st);
}
