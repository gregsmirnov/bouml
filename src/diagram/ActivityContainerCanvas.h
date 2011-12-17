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

#ifndef ACTIVITYCONTAINERCANVAS_H
#define ACTIVITYCONTAINERCANVAS_H





#include "DiagramCanvas.h"

class ActivityContainerCanvas : public DiagramCanvas {
  protected:
    ActivityContainerCanvas(UmlCanvas * canvas, int id)
      : DiagramCanvas(canvas, id) {
    }
  
  public:
    ActivityContainerCanvas(BrowserNode * bn, UmlCanvas * canvas,
			    int x, int y, int w, int h, int id) 
      : DiagramCanvas(bn, canvas, x, y, w, h, id) {
    }

    void force_inside(DiagramItem * di, QCanvasItem * ci, BooL & need_sub_upper);
    void force_sub_inside(QCanvasItemList & all, BooL & need_sub_upper);
    void resize_to_contain(QCanvasItemList & all, BooL & need_sub_upper);
    void force_sub_upper(QCanvasItemList & all);
    virtual void prepare_for_move(bool on_resize);
    static bool force_inside(DiagramCanvas * elt, bool part);
};

#endif
