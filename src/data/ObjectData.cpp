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





#include "ObjectData.h"
#include "BrowserClass.h"
#include "myio.h"
#include "ToolCom.h"

ObjectData::ObjectData()
    : ordering(UmlUnordered), is_control(FALSE) {
}

ObjectData::ObjectData(ObjectData * model) {
  set_type(model->type);
  multiplicity = model->multiplicity;
  in_state = model->in_state;
  uml_selection = model->uml_selection;
  cpp_selection = model->cpp_selection;
  java_selection = model->java_selection;
  ordering = model->ordering;
  is_control = model->is_control;
}

ObjectData::~ObjectData() {
}

const char * ObjectData::get_selection(DrawingLanguage lang) const {
  switch (lang) {
  case CppView:
    return cpp_selection;
  case JavaView:
    return java_selection;
  default:
    // uml
    return uml_selection;
  }
}

void ObjectData::set_type(BrowserClass * c) {
  if (type.type != c) {
    if (type.type != 0)
      do_disconnect(type.type);
    if (c != 0)
      do_connect(c);
    type.explicit_type = 0;
    type.type = c;
  }
}

void ObjectData::set_type(const AType & t) {
  if (t.type != 0)
    set_type(t.type);
  else {
    if (type.type != 0) {
      do_disconnect(type.type);
      type.type = 0;
    }
    type.explicit_type = t.explicit_type;
  }
}

void ObjectData::send_uml_def(ToolCom * com) {
  type.send_def(com);
  com->write_string(multiplicity);
  com->write_string(in_state);
  com->write_string(uml_selection);
  com->write_char(ordering);
  com->write_bool(is_control);
}

void ObjectData::send_cpp_def(ToolCom * com) {
  com->write_string(cpp_selection);
}

void ObjectData::send_java_def(ToolCom * com) {
  com->write_string(java_selection);
}

bool ObjectData::tool_cmd(ToolCom * com, const char * args) {
  // note : write access already check
  // warning : setTypeCmd already managed
  switch ((unsigned char) args[-1]) {
  case setTypeCmd:
    {
      AType t;
      
      com->get_type(t, args);
      set_type(t);
    }
    break;
  case setMultiplicityCmd:
    multiplicity = args;
    break;
  case setInStateCmd:
    in_state = args;
    break;
  case setUmlActivityCmd:
    uml_selection = args;
    break;
  case setCppActivityCmd:
    cpp_selection = args;
    break;
  case setJavaActivityCmd:
    java_selection = args;
    break;
  case setOrderingCmd:
    ordering  = (UmlOrderingKind) *args;
    break;
  case setFlagCmd:
    is_control = *args != 0;
    break;
  default:
    return FALSE;
  }

  return TRUE;
}

void ObjectData::save(QTextStream & st, QString & warning) const {
  type.save(st, warning, " type ", " explicit_type ");
  nl_indent(st);
  
  if (!multiplicity.isEmpty()) {
    st << " multiplicity ";
    save_string(multiplicity, st);
    st << ' ';
  }
  
  st << stringify(ordering) << ' ';
  
  if (!in_state.isEmpty()) {
    st << " in_state ";
    save_string(in_state, st);
    st << ' ';
  }

  if (is_control)
    st << "is_control";
  
  if (!uml_selection.isEmpty()) {
    nl_indent(st);
    st << "uml_selection ";
    save_string(uml_selection, st);
  }
  
  if (!cpp_selection.isEmpty()) {
    nl_indent(st);
    st << "cpp_selection ";
    save_string(cpp_selection, st);
  }
  
  if (!java_selection.isEmpty()) {
    nl_indent(st);
    st << "java_selection ";
    save_string(java_selection, st);
  }
}

void ObjectData::read(char * & st, char * & k) { 
  AType t;
  
  t.read(st, "type", "explicit_type", k);	// k unchanged
  set_type(t);
  
  k = read_keyword(st);
  
  if (!strcmp(k, "multiplicity")) {
    multiplicity = read_string(st);
    k = read_keyword(st);
  }
  
  ordering = ::ordering(k);
  
  k = read_keyword(st);
  
  if (!strcmp(k, "in_state")) {
    in_state = read_string(st);
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "is_control")) {
    is_control = TRUE;
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "uml_selection")) {
    uml_selection = read_string(st);
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "cpp_selection")) {
    cpp_selection = read_string(st);
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "java_selection")) {
    java_selection = read_string(st);
    k = read_keyword(st);
  }
}
