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

#include "UseCaseData.h"
#include "BrowserClass.h"
#include "ClassData.h"
#include "BrowserUseCase.h"
#include "UseCaseDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"
#include "strutil.h"

UseCaseData::UseCaseData() {
}

// doesn't copy associated classes or artifacts
UseCaseData::UseCaseData(UseCaseData * model, BrowserNode * bn)
    : SimpleData(model) {
  browser_node = bn;
  extension_points = model->extension_points;
}

UseCaseData::~UseCaseData() {
}

void UseCaseData::edit() {
  setName(browser_node->get_name());
  
  (new UseCaseDialog(this))->show();
}

void UseCaseData::set_extension_points(QString s) {
  extension_points = s;
}

//

void UseCaseData::send_uml_def(ToolCom * com, BrowserNode * bn, 
			       const QString & comment) {
  BasicData::send_uml_def(com, bn, comment);
  if (com->api_format() >= 28)
    com->write_string(extension_points);
}

bool UseCaseData::tool_cmd(ToolCom * com, const char * args,
			   BrowserNode * bn, const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case replaceExceptionCmd:
	if (!bn->is_writable() && !root_permission())
	  com->write_ack(FALSE);
	else {
	  set_extension_points(args);
	  com->write_ack(TRUE);
	}
	return TRUE;
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

void UseCaseData::save(QTextStream & st, QString & warning) const {
  if (!extension_points.isEmpty()) {
    nl_indent(st);
    st << "extension_points ";
    save_string(fromUnicode(extension_points), st);  
  }
  
  BasicData::save(st, warning);
}

void UseCaseData::read(char * & st, char * & k) {
  if (!strcmp(k, "extension_points")) {
    extension_points = toUnicode(read_string(st));
    k = read_keyword(st);
  }
  
  BasicData::read(st, k);	// updates k
}
