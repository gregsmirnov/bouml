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





#include "BrowserClass.h"
#include "ClassData.h"
#include "FormalParamData.h"
#include "myio.h"
#include "ToolCom.h"

FormalParamData::FormalParamData(const FormalParamData& f)
    : QObject(0) {
  *this = f;
}

FormalParamData& FormalParamData::operator=(const FormalParamData& f) {
  name = f.name;
  type = f.type;
  set_default_value(f.default_value.type, f.default_value.explicit_type);
  set_extends(f.extends.type, f.extends.explicit_type);

  return *this;
}

void FormalParamData::set_default_value(BrowserClass * t, const QString & e) {
  if (default_value.type != t) {
    if (default_value.type != 0)
      disconnect(default_value.type->get_data(), SIGNAL(deleted()),
		 this, SLOT(on_delete_default()));
    if ((default_value.type = t) != 0)
      connect(default_value.type->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete_default()));
  }
  
  default_value.explicit_type = e;
}

void FormalParamData::set_extends(BrowserClass * t, const QString & e) {
  if (extends.type != t) {
    if (extends.type != 0)
      disconnect(extends.type->get_data(), SIGNAL(deleted()),
		 this, SLOT(on_delete_extends()));
    if ((extends.type = t) != 0)
      connect(extends.type->get_data(), SIGNAL(deleted()),
	      this, SLOT(on_delete_extends()));
  }
  
  extends.explicit_type = e;
}

void FormalParamData::on_delete_default() {
  if (default_value.type && default_value.type->deletedp()) {
    default_value.explicit_type = default_value.type->get_name();
    default_value.type = 0;
  }
}

void FormalParamData::on_delete_extends() {
  if (extends.type && extends.type->deletedp()) {
    extends.explicit_type = extends.type->get_name();
    extends.type = 0;
  }
}

void FormalParamData::send_uml_def(ToolCom * com) {
  com->write_string(name);
  com->write_string(type);
  default_value.send_def(com);
  if (com->api_format() >= 20)
    extends.send_def(com);
}
      
void FormalParamData::read(ToolCom * com, const char *& args) {
  name = com->get_string(args);
  type = com->get_string(args);
  
  AType t1;
  
  com->get_type(t1, args);
  set_default_value(t1.type, t1.explicit_type);
  
  if (com->api_format() >= 20) {
    AType t2;
    
    com->get_type(t2, args);
    set_extends(t2.type, t2.explicit_type);
  }
}

void FormalParamData::skip(ToolCom * com, const char *& args)
{
  com->get_string(args);
  com->get_string(args);
  com->skip_type(args);
  if (com->api_format() >= 20)
    com->skip_type(args);
}

void FormalParamData::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  st << "formal name ";
  save_string(name, st);
  st << " type ";
  save_string(type, st);
  default_value.save(st, warning, " default_value ", " explicit_default_value ");
  nl_indent(st);	// default_value may produce a comment
  extends.save(st, warning, "  extends ", "  explicit_extends ");
}

void FormalParamData::read(char * & st) {
  read_keyword(st, "formal");
  read_keyword(st, "name");
  name = read_string(st);
  read_keyword(st, "type");
  type = read_string(st);
  
  AType t1;
  
  t1.read(st, "default_value", "explicit_default_value");
  set_default_value(t1.type, t1.explicit_type);
  
  if (read_file_format() >= 24) {
    AType t2;
    
    t2.read(st, "extends", "explicit_extends");
    set_extends(t2.type, t2.explicit_type);
  }
}
