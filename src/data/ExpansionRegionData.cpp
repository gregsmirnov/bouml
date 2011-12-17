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

#include "BrowserExpansionRegion.h"
#include "ExpansionRegionData.h"
#include "ExpansionRegionDialog.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

ExpansionRegionData::ExpansionRegionData()
    : must_isolate(FALSE), mode(UmlIterative) {
}

ExpansionRegionData::ExpansionRegionData(ExpansionRegionData * model, BrowserNode * bn)
    : SimpleData(model) {
  browser_node = bn;
  must_isolate = model->must_isolate;
  mode = model->mode;
}

ExpansionRegionData::~ExpansionRegionData() {
}

void ExpansionRegionData::edit() {
  setName(browser_node->get_name());
    
  (new ExpansionRegionDialog(this))->show();
}

//
  
void ExpansionRegionData::send_uml_def(ToolCom * com, BrowserNode * bn,
				       const QString & comment) {
  SimpleData::send_uml_def(com, bn, comment);
  com->write_bool(must_isolate);
  com->write_char(mode);
}

bool ExpansionRegionData::tool_cmd(ToolCom * com, const char * args,
				   BrowserNode * bn, const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setFlagCmd:
	must_isolate = *args != 0;
	break;
      case setDefCmd:
	mode = (UmlExpansionKind) *args;
	break;
      default:
	return BasicData::tool_cmd(com, args, bn, comment);
      }
      
      // ok case
      bn->package_modified();
      modified();
      com->write_ack(TRUE);
    }
  }
  else
    return BasicData::tool_cmd(com, args, bn, comment);
  
  return TRUE;
}

//

void ExpansionRegionData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);
  nl_indent(st);

  if (must_isolate)
    st << "must_isolate ";

  st << stringify(mode);
}

void ExpansionRegionData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  if (! strcmp(k, "must_isolate")) {
    must_isolate = TRUE;
    k = read_keyword(st);
  }

  mode = expansion_mode_kind(k);
  k = read_keyword(st);
}
