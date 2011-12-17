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

#include "BrowserActivityPartition.h"
#include "ActivityPartitionData.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"

ActivityPartitionData::ActivityPartitionData()
    : is_dimension(FALSE), is_external(FALSE), represents(0) {
}

ActivityPartitionData::ActivityPartitionData(ActivityPartitionData * model, BrowserNode * bn)
    : SimpleData(model) {
  browser_node = bn;
  is_dimension = model->is_dimension;
  is_external = model->is_external;
  represents = model->represents;
}

ActivityPartitionData::~ActivityPartitionData() {
}

//

void ActivityPartitionData::send_uml_def(ToolCom * com, BrowserNode * bn,
					 const QString & comment) {
  SimpleData::send_uml_def(com, bn, comment);
  com->write_bool(is_dimension);
  com->write_bool(is_external);
  if (represents != 0)
    represents->write_id(com);
  else
    com->write_id(0);
}

bool ActivityPartitionData::tool_cmd(ToolCom * com, const char * args,
				     BrowserNode * bn,
				     const QString & comment) {
  if (((unsigned char) args[-1]) >= firstSetCmd) {
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      switch ((unsigned char) args[-1]) {
      case setMultiplicityCmd:
	is_dimension = (*args != 0);
	break;
      case setIsCppExternalCmd:
	is_external = (*args != 0);
	break;
      case setDerivedCmd:
	represents = (BrowserNode *) com->get_id(args);
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

void ActivityPartitionData::post_load() {
  if ((represents != 0) && represents->is_undefined())
    represents = 0;
}

void ActivityPartitionData::save(QTextStream & st, QString & warning) const {
  BasicData::save(st, warning);

  bool nl = FALSE;

  if (is_dimension) {
    nl_indent(st);
    nl = TRUE;
    st << "is_dimension";
  }

  if (is_external) {
    if (nl)
      st << " ";
    else
      nl_indent(st);
    nl = TRUE;
    st << "is_external";
  }
  
  if ((represents != 0) && !represents->deletedp()) {
    if (nl)
      st << " ";
    else
      nl_indent(st);
    st << "represents ";
    represents->save(st, TRUE, warning);
  }
}

void ActivityPartitionData::read(char * & st, char * & k) {
  BasicData::read(st, k);	// updates k
  if (! strcmp(k, "is_dimension")) {
    is_dimension = TRUE;
    k = read_keyword(st);
  }
  
  if (! strcmp(k, "is_external")) {
    is_external = TRUE;
    k = read_keyword(st);
  }
  
  if (! strcmp(k, "represents")) {
    k = read_keyword(st);
    
    if ((represents = BrowserNode::read_any_ref(st, k)) == 0)
      unknown_keyword(k);
    
    k = read_keyword(st);
  }
}
