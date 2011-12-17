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





#include "ExceptionData.h"
#include "BrowserClass.h"
#include "myio.h"
#include "ToolCom.h"

void ExceptionData::set_type(const AType & t) {
  type = t;
}

void ExceptionData::send_uml_def(ToolCom * com) {
  type.send_def(com);
}

void ExceptionData::save(QTextStream & st, QString & warning) const {
  nl_indent(st);
  type.save(st, warning, "  exception ", "  explicit_exception ");
}

void ExceptionData::read(char * & st, char * & k) {
  type.read(st, "exception", "explicit_exception", k);

  k = read_keyword(st);
}
