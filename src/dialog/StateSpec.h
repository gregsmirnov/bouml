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

#ifndef STATESPEC_H
#define STATESPEC_H

#include "UmlEnum.h"

class StateSpec {
  public:
    enum kind {
      is3states, isClassDrawingMode, isDrawingLanguage, isShowContextMode, isMemberWidth
    };
  
    QString name;
    void * state;
    kind who;
    
    void set(QString n, Uml3States * s) {
      name = n;
      state = s;
      who = is3states;
    };
    void set(QString n, ClassDrawingMode * s) {
      name = n;
      state = s;
      who = isClassDrawingMode;
    };
    void set(QString n, DrawingLanguage * s) {
      name = n;
      state = s;
      who = isDrawingLanguage;
    };
    void set(QString n, ShowContextMode * s) {
      name = n;
      state = s;
      who = isShowContextMode;
    };
    void set(QString n, char * s) {
      name = n;
      state = s;
      who = isMemberWidth;
    };
    
    void set_state(int v);
};

class StateSpecVector {
  public:
    StateSpecVector() : _size(0), _v(0) {}
    StateSpecVector(int s) : _size(s) { _v = new StateSpec[s]; }
    ~StateSpecVector() { if (_v) delete [] _v; }
    int size() const { return _size; }
    void resize(int s);
    StateSpec & operator[] (int i) const { return _v[i]; }
    StateSpec & at (int i) const { return _v[i]; }
    
  private:
    int _size;
    StateSpec * _v;
};

#endif
