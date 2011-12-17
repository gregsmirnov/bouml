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

#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include <qtextstream.h>

#include "AType.h"
#include "UmlEnum.h"

class BrowserClass;

class ObjectData {
private:
  AType type;

public:
  SharedStr multiplicity;
  MyStr in_state;
  MyStr uml_selection;
  MyStr cpp_selection;
  MyStr java_selection;
  UmlOrderingKind ordering : 8;
  bool is_control : 1;
  
  ObjectData();
  ObjectData(ObjectData * model);
  virtual ~ObjectData();
  
  virtual void do_connect(BrowserClass * c) = 0;
  virtual void do_disconnect(BrowserClass * c) = 0;
  
  const AType & get_type() const { return type; };
  void set_type(BrowserClass * c);
  void set_type(const AType & t);
  const char * get_multiplicity() const { return multiplicity; };
  const char * get_in_state() const { return in_state; };
  const char * get_selection(DrawingLanguage) const;
  UmlOrderingKind get_ordering() const { return ordering; }
  bool get_is_control() const { return is_control; }
    
  void send_uml_def(ToolCom * com);
  virtual void send_cpp_def(ToolCom * com);
  virtual void send_java_def(ToolCom * com);
  bool tool_cmd(ToolCom * com, const char * args);
  
  void save(QTextStream &, QString & warning) const;
  void read(char * &, char * &);
};

#endif
