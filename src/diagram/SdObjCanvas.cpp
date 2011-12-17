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





#include "SdObjCanvas.h"
#include "SdLifeLineCanvas.h"
#include "myio.h"
#include "translate.h"

SdObjCanvas::SdObjCanvas(BrowserNode * bn, UmlCanvas * canvas,
			 int x, int we, int he, int id)
    : DiagramCanvas(bn, canvas, x, SDOBJ_VERT_MARGIN, we, he, id),
      life_line(0), mortal(FALSE) {
  life_line = new SdLifeLineCanvas(canvas, this);
}

SdObjCanvas::~SdObjCanvas() {
}

void SdObjCanvas::delete_it() {
  life_line->delete_it();
  DiagramCanvas::delete_it();
}

void SdObjCanvas::change_scale() {
  DiagramCanvas::change_scale();
  
  if (life_line)
    life_line->update_pos();
}

void SdObjCanvas::moveBy(double dx, double dy) {
  if (((y() + dy) / the_canvas()->zoom()) < SDOBJ_VERT_MARGIN)
    dy = SDOBJ_VERT_MARGIN * the_canvas()->zoom() - y();
  else if (life_line) {
    double maxy = life_line->instance_max_y() - SDOBJ_VERT_MARGIN;
    
    if ((y() + dy) > maxy)
      dy = maxy - y();
  }
  
  DiagramCanvas::moveBy(dx, dy);
  
  if (life_line && !life_line->selected())
    life_line->update_pos();
}

QString SdObjCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : TR("illegal");
}

QString SdObjCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  return (l == UmlAnchor) ? dest->may_start(l) : TR("illegal");
}

void SdObjCanvas::set_mortal(bool y) {
  mortal = y;
  life_line->update_instance_dead();
}

bool SdObjCanvas::copyable() const {
  // do not check selected()
  return life_line->copyable();
}

void SdObjCanvas::save(QTextStream & st) const {
  if (mortal)
    st << "  mortal";
  save_xyz(st, this, "  xyz");
  if (life_line->is_masked())
    st << " life_line_masked";
  st << " life_line_z " << life_line->z();
}

void SdObjCanvas::read(char * & st, const char * k) {
  if (!strcmp(k, "mortal")) {
    mortal = TRUE;
    k = read_keyword(st);
  }
  
  if (!strcmp(k, "xyz")) {
    // new version
    read_xyz(st, this);
    k = read_keyword(st);
    if (!strcmp(k, "life_line_masked")) {
      life_line->set_masked(TRUE);
      k = read_keyword(st);
    }
    if (strcmp(k, "life_line_z"))
      wrong_keyword(k, "life_line_z");
    life_line->setZ(read_double(st));
  }
  else if (!strcmp(k, "xy"))
    read_xy(st, this);
  else
    wrong_keyword(k, "xy/xyz");
  set_center100();
}
