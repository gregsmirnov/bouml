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
#include <qcursor.h>
#include <qpopupmenu.h> 

#include "FragmentSeparatorCanvas.h"
#include "FragmentCanvas.h"
#include "MenuTitle.h"
#include "myio.h"
#include "translate.h"

#define LIFE_LINE_HEIGHT 7

FragmentSeparatorCanvas::FragmentSeparatorCanvas(UmlCanvas * canvas, FragmentCanvas * f,
						 double vp)
    : DiagramCanvas(0, canvas, 0, 0, LIFE_LINE_HEIGHT, 0, -1),
      fragment(f), vpos(vp) {
  if (canvas->paste())
    // must not be selected else the move done after if wrong
    canvas->unselect(this);
  else
    compute_position();
  
  show();
}

FragmentSeparatorCanvas::~FragmentSeparatorCanvas() {
}

void FragmentSeparatorCanvas::delete_it() {
  fragment->remove_it(this);
  DiagramCanvas::delete_it();
}

void FragmentSeparatorCanvas::compute_position() {
  double zoom = the_canvas()->zoom();
  double fy = fragment->y();
  double fh = fragment->height();
  double offset = LIFE_LINE_HEIGHT / 2.0 * zoom;
  double new_x = fragment->x();
  double new_y = fy + fh * vpos;
  
  if (new_y <= fy)
    new_y = fy + 1;
  else if (new_y >= fy + fh - 1)
    new_y = fy + fh - 2;
  
  setZ(fragment->z() + 1);
  setSize(fragment->width(), (int) (LIFE_LINE_HEIGHT*zoom));
  QCanvasRectangle::moveBy(new_x - x(), new_y - offset - y());
}

void FragmentSeparatorCanvas::update() {
  hide();
  compute_position();
  show();
  canvas()->update();
}

void FragmentSeparatorCanvas::change_scale() {
  // do nothing, update called by its fragment
}

void FragmentSeparatorCanvas::drawShape(QPainter & p) {
  p.setBackgroundMode(::Qt::TransparentMode);
  
  p.setPen(::Qt::DashLine);
  
  int m = (int) (fragment->y() + fragment->height() * vpos);
  
  p.drawLine((int) fragment->x(), m,
	     (int) fragment->x() + fragment->width() - 1, m);
  
  p.setPen(::Qt::SolidLine);


  FILE * fp = svg();

  if (fp != 0)
    fprintf(fp, "<g>\n\t<line stroke=\"black\" stroke-dasharray=\"20,4\" stroke-opacity=\"1\""
	    " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n</g>\n",
	    (int) fragment->x(), m,
	    (int) fragment->x() + fragment->width() - 1, m);
  
  if (selected())
    show_mark(p, rect());
}

void FragmentSeparatorCanvas::moveBy(double, double dy) {
  // moved by user
  if (! fragment->selected()) {
    double zoom = the_canvas()->zoom();
    double offset = LIFE_LINE_HEIGHT / 2.0 * zoom;
    float new_y = y() + dy + offset;
    double fy = fragment->y();
    double fh = fragment->height();
    
    if (new_y <= (fy + 1))
      new_y = fy + 2;
    else if (new_y >= fy + fh - 1)
      new_y = fy + fh - 2;
  
    vpos = (new_y - fy) / fh;
    QCanvasRectangle::moveBy(0, new_y - offset - y());
  }
}

UmlCode FragmentSeparatorCanvas::type() const {
  return UmlFragmentSeparator;
}

void FragmentSeparatorCanvas::delete_available(BooL &, BooL & out_model) const {
  out_model |= TRUE;
}

DiagramItem::LineDirection FragmentSeparatorCanvas::allowed_direction(UmlCode) {
  return DiagramItem::Horizontal;
}

QString FragmentSeparatorCanvas::may_start(UmlCode &) const {
  return TR("illegal");
}

QString FragmentSeparatorCanvas::may_connect(UmlCode &, const DiagramItem *) const {
  return TR("illegal");
}

void FragmentSeparatorCanvas::connexion(UmlCode, DiagramItem *,
					const QPoint &, const QPoint &) {
  // not allowed
}

void FragmentSeparatorCanvas::open() {
}

void FragmentSeparatorCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  
  m.insertItem(new MenuTitle(TR("Separator"), m.font()), -1);
  m.insertSeparator();
  m.insertItem(TR("Remove from diagram"), 1);
  
  if (m.exec(QCursor::pos()) == 1) {
    delete_it();
    package_modified();
  }
}

void FragmentSeparatorCanvas::save(QTextStream & st, bool, QString &) const {
  nl_indent(st);
  st << "separator " << (int) (vpos * 10000);
}

FragmentSeparatorCanvas *
  FragmentSeparatorCanvas::read(char * & st, UmlCanvas * canvas,
				char * k, FragmentCanvas * f)
{
  if (strcmp(k, "separator"))
    wrong_keyword(k, "separator");
  
  FragmentSeparatorCanvas * result =
    new FragmentSeparatorCanvas(canvas, f, read_double(st) / 10000);
  
  result->compute_position();
  result->show();
  return result;
}

void FragmentSeparatorCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(vpos, b);
  ::save(fragment, b);
}

void FragmentSeparatorCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(vpos, b);
  fragment = (FragmentCanvas *) ::load_item(b);
}
