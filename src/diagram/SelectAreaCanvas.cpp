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





#include <qpainter.h>

#include "SelectAreaCanvas.h"

SelectAreaCanvas::SelectAreaCanvas(int x, int y, int width, int height,
				   QCanvas * canvas)
    : QCanvasRectangle(x, y, width, height,canvas) {
}
  
void SelectAreaCanvas::drawShape(QPainter & p) {
  if (! visible()) return;
  
  p.setPen(::Qt::DotLine);
  QCanvasRectangle::drawShape(p);
  p.setPen(::Qt::SolidLine);
}
