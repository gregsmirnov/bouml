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





#include "ParamData.h"
#include "BrowserClass.h"
#include "myio.h"
#include "ToolCom.h"

ParamData::ParamData() : dir(UmlInOut) {
}

void ParamData::set_name(const char * s) {
  name = s;
}

void ParamData::set_dir(UmlParamDirection d) {
  dir = d;
}

void ParamData::set_type(const AType & t) {
  type = t;
}

void ParamData::set_default_value(const char * str) {
  default_value = str;
}

QString ParamData::definition(bool withdir, bool withname,
			      ShowContextMode mode) const {
  QString t;
  
  if (type.type == 0)
    t = (const char *) type.explicit_type;
  else
    t = type.type->contextual_name(mode);
  
  if (withdir) {
    QString r = stringify(dir) + QString(" ");
    
    if (withname)
      r += name + " : ";
  
    r += t;
    
    QString v = default_value;
    
    v = v.simplifyWhiteSpace();
    
    return ((dir == UmlOut) || v.isEmpty())
      ? r
      : r + " = " + v;
  }
  else if (withname)
    return name + QString(" : ") + t;
  else
    return t;
}

void ParamData::send_uml_def(ToolCom * com) {
  com->write_char(dir);
  type.send_def(com);
  com->write_string(name);
  com->write_string(default_value);
}

void ParamData::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  st << "  param " << stringify(dir) << " name ";
  save_string(name, st);
  type.save(st, warning, " type ", " explicit_type ");
  if (!default_value.isEmpty()) {
    nl_indent(st);
    st << "    defaultvalue ";
    save_string(default_value, st);
  }
}

void ParamData::read(char * & st, char * & k) {
  if (strcmp(k, "param"))
    wrong_keyword(k, "param");
  
  dir = direction(read_keyword(st));
  
  read_keyword(st, "name");
  name = read_string(st);
  type.read(st, "type", "explicit_type");
  k = read_keyword(st);
  if (!strcmp(k, "defaultvalue")) {
    default_value = read_string(st);
    k = read_keyword(st);
  }
}
