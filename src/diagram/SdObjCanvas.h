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

#ifndef SDOBJCANVAS_H
#define SDOBJCANVAS_H

#include "DiagramCanvas.h"

class SdLifeLineCanvas;
class BrowserClass;

#define SDOBJ_VERT_MARGIN 4

class SdObjCanvas : public DiagramCanvas {
  protected:
    SdLifeLineCanvas * life_line;
    bool mortal;
  
  public:
    SdObjCanvas(BrowserNode * bn, UmlCanvas * canvas, int x,
		int w, int h, int id);
    virtual ~SdObjCanvas();
    
    virtual void delete_it();

    SdLifeLineCanvas * get_life_line() const { return life_line; };
    bool is_mortal() { return mortal; }
    void set_mortal(bool y);
    
    virtual BrowserClass * get_class() const = 0;
    
    virtual void moveBy(double dx, double dy);
    
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    void change_scale();
    virtual bool copyable() const;
    
  protected:
    void read(char * & st, const char * k);
    void save(QTextStream & st) const;
};

#endif

