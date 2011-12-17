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
#include "ActualParamData.h"
#include "myio.h"
#include "ToolCom.h"

ActualParamData::ActualParamData(BrowserClass * p, unsigned r)
    : super(p), rank(r) {
}

ActualParamData::ActualParamData(const ActualParamData& p)
    : QObject(0), super(p.super),  rank(p.rank) {
  set_value(p.value);
}

QString ActualParamData::get_name(bool full) const {
  QString s = ((ClassData *) super->get_data())->get_formalparam_name(rank);
  
  return (full)
    ? super->full_name() + " : " + s
    : s;
}

void ActualParamData::set_value(const AType & t) {
  if (value.type != t.type) {
    if (value.type != 0)
      disconnect(value.type->get_data(), 0, this, 0);
    if ((value.type = t.type) != 0)
      connect(value.type->get_data(), SIGNAL(deleted()), this, SLOT(on_delete()));
  }
    
  value.explicit_type = t.explicit_type;
}

void ActualParamData::on_delete() {
  if (value.type && value.type->deletedp()) {
    value.explicit_type = value.type->get_name();
    value.type = 0;
  }
}

void ActualParamData::send_uml_def(ToolCom * com) {
  super->write_id(com);
  com->write_unsigned(rank);
  value.send_def(com);
}

void ActualParamData::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  st << "actual class ";
  super->save(st, TRUE, warning);
  nl_indent(st);
  st << "  rank " << rank << ' ';
  value.save(st, warning, "value ", "explicit_value ");
}

ActualParamData * ActualParamData::read(char * & st)
{
  read_keyword(st, "actual");
  read_keyword(st, "class");
  
  BrowserClass * p = BrowserClass::read_ref(st);
  
  read_keyword(st, "rank");
  
  ActualParamData * result = new ActualParamData(p, read_unsigned(st));
  
  AType t;
  
  t.read(st, "value", "explicit_value");
  
  result->set_value(t);
  
  return result;
}
