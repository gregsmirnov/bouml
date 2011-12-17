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

#include <qstring.h>

#include "StateSpec.h"

void StateSpec::set_state(int v) {
  switch (who) {
  case is3states:
    *((Uml3States *) state) = (Uml3States) v;
    break;
  case isClassDrawingMode:
    *((ClassDrawingMode *) state) = (ClassDrawingMode) v;
    break;
  case isDrawingLanguage:
    *((DrawingLanguage *) state) = (DrawingLanguage) v;
    break;
  case isShowContextMode:
    *((ShowContextMode *) state) = (ShowContextMode) v;
    break;
  default:
    *((char *) state) = v;
  }
}

void StateSpecVector::resize(int s) {
  StateSpec * v = new StateSpec[s];
  
  if (_v != 0) {
    for (int i = 0; i != _size; i += 1)
      v[i] = _v[i];
    
    delete [] _v;
  }
  _v = v;
  _size = s;
}
