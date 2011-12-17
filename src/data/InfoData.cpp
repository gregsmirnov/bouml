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





#include "InfoData.h"
#include "myio.h"
#include "ToolCom.h"

void InfoData::save(QTextStream & st, const char * s1, const char * s2) const {
  indent(+1);
  
  if (!first.isEmpty()) {
    nl_indent(st);
    st << s1 << " ";
    save_string(first, st);
  }
  if (!second.isEmpty()) {
    nl_indent(st);
    st << s2 << " ";
    save_string(second, st);
  }
  
  indent(-1);
}

void InfoData::read(char * & st, char * & k, const char * s1, const char * s2) {
  if (!strcmp(k, s1)) {
    first = read_string(st);
    k = read_keyword(st);
  }
  if (!strcmp(k, s2)) {
    second = read_string(st);
    k = read_keyword(st);
  }
}

void InfoData::send_def(ToolCom * com) {
  com->write_string(first);
  com->write_string(second);
}
