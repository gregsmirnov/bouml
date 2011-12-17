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





#include "PinParamData.h"
#include "BrowserClass.h"
#include "myio.h"
#include "ToolCom.h"

PinParamData::PinParamData()
    : unique(FALSE), exception(FALSE), stream(FALSE),
      dir(UmlIn), effect(UmlNoEffect) {
}

PinParamData::PinParamData(PinParamData * model) : ObjectData(model) {
  unique = model->unique;
  exception = model->exception;
  stream = model->stream;
  dir = model->dir;
  effect = model->effect;
}

void PinParamData::send_uml_def(ToolCom * com) {
  ObjectData::send_uml_def(com);
  com->write_bool(unique);
  com->write_bool(exception);
  com->write_bool(stream);
  com->write_char(dir);
  com->write_char(effect);
}

bool PinParamData::tool_cmd(ToolCom * com, const char * args, BooL & ack) {
  // note : write access already check
  ack = TRUE;

  switch ((unsigned char) args[-1]) {
  case setUniqueCmd:
    unique = *args != 0;
    break;
  case replaceExceptionCmd:
    if (stream)
      ack = FALSE;
    else
      exception = *args;
    break;
  case setStreamCmd:
    if (exception)
      ack = FALSE;
    else
      stream = *args;
    break;
  case setIdlDirCmd:
    dir = (UmlParamDirection) *args;
    break;
  case replaceParameterCmd:
    effect  = (UmlParamEffect) *args;
    break;
  default:
    return ObjectData::tool_cmd(com, args);
  }

  return TRUE;
}

void PinParamData::save(QTextStream & st, QString & warning) const {
  ObjectData::save(st, warning);
  
  nl_indent(st);
  
  st << stringify(dir);
  
  if (effect != UmlNoEffect)
    st << " effect " << stringify(effect);
  
  if (unique)
    st << " unique";
  
  if (exception)
    st << " exception";
  
  if (stream)
    st << " stream";
}

void PinParamData::read(char * & st, char * & k) {
  ObjectData::read(st, k);	// update k
  
  dir = direction(k);
  
  k = read_keyword(st);
  
  if (! strcmp(k, "effect")){
    effect = ::effect(read_keyword(st));
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "unique")) {
    unique = TRUE;
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "exception")) {
    exception = TRUE;
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "stream")) {
    stream = TRUE;
    k = read_keyword(st);
  }
}
