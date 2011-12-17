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





#include <qtextstream.h>

#include "AType.h"
#include "BrowserClass.h"
#include "ToolCom.h"
#include "myio.h"

const char * AType::get_type() const {
  return (type != 0) ? type->get_name() : explicit_type; 
}

QString AType::get_type(ShowContextMode mode) const {
  return (type == 0)
    ? QString((const char *) explicit_type)
    : type->contextual_name(mode);
}

QString AType::get_full_type() const {
  return (type != 0) ? type->full_name(TRUE)
		     : QString((const char *) explicit_type); 
}

void AType::send_def(ToolCom * com) const {
  if (type == 0) {
    com->write_id(0);
    com->write_string(explicit_type);
  }
  else if (type->deletedp()) {
    com->write_id(0);
    com->write_string(0);
  }
  else
    type->write_id(com);
}

// returns FALSE on error

void AType::save(QTextStream & st, QString & warning,
		 const char * t, const char * ex) const {
  if (type != 0) {
    if (type->deletedp()) {
      // theo not possible
      st << ex;
      save_string(type->get_name(), st);
    }
    else{
      st << t;
      type->save(st, TRUE, warning);
    }
  }
  else {
    st << ex;
    save_string(explicit_type, st);
  }
}

void AType::read(char * & st, const char * t, const char * ex) {
  char * k = read_keyword(st);
  
  if (!strcmp(k, t)) {
    type = BrowserClass::read_ref(st);
    explicit_type = QString::null;
  }
  else if (!strcmp(k, ex)) {
    type = 0;
    explicit_type = read_string(st);
  }
  else 
    wrong_keyword(k, QString(t) + '/' + ex);
}

void AType::read(char * & st, const char * t, const char * ex, const char * k) {
  if (!strcmp(k, t)) {
    type = BrowserClass::read_ref(st);
    explicit_type = QString::null;
  }
  else if (!strcmp(k, ex)) {
    type = 0;
    explicit_type = read_string(st);
  }
  else 
    wrong_keyword(k, QString(t) + '/' + ex);
}
