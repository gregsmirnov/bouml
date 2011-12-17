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





#include <math.h>
#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "ArrowJunctionCanvas.h"
#include "ArrowCanvas.h"
#include "UmlCanvas.h"
#include "BrowserClass.h"
#include "myio.h"
#include "MenuTitle.h"
#include "BrowserDiagram.h"
#include "UmlPixmap.h"
#include "LabelCanvas.h"
#include "BasicData.h"
#include "translate.h"

ArrowJunctionCanvas::ArrowJunctionCanvas(UmlCanvas * canvas, int x, int y,
					 BrowserClass * i, int id) 
    : DiagramCanvas(0, canvas, x, y, ARROW_JUNCTION_SIZE, ARROW_JUNCTION_SIZE, id),
      interface(i) {
  browser_node = canvas->browser_diagram();
  
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  QString s = i->get_name();
  
  label = new LabelCanvas(s, canvas,
			  x - fm.width(s)/2 + ARROW_JUNCTION_SIZE/2,
			  y + ARROW_JUNCTION_SIZE);

  connect(i->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(i->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}

ArrowJunctionCanvas::~ArrowJunctionCanvas() {
}

void ArrowJunctionCanvas::delete_it() {
  unconnect();
  DiagramCanvas::delete_it();
}

void ArrowJunctionCanvas::unconnect() {
  if (interface != 0) {
    disconnect(interface->get_data(), 0, this, 0);
    interface = 0;
  }
}

void ArrowJunctionCanvas::modified() {
  if ((interface != 0) && (interface->get_name() != label->get_name()))
    label->set_name(interface->get_name());
}

void ArrowJunctionCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void ArrowJunctionCanvas::change_scale() {
  // the size is not modified
  QCanvasRectangle::setVisible(FALSE);
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void ArrowJunctionCanvas::draw(QPainter & p) {
  if (!visible()) 
    return;
  
  ArrowCanvas * a;
  FILE * fp = svg();
  
  for (a = lines.first(); a != 0; a = lines.next()) {
    switch (a->type()) {
    case UmlRequired:
      {
	QRect r = rect();
	int wh = r.width() - 2;
	int startangle = a->get_point(1).x(); // degree * 16
	
	p.drawArc(r.x()+1, r.y()+1,
		  wh, wh,
		  startangle, 180*16);
	if (fp != 0) {
	  int radius = r.width()/2;

	  QPoint ce = r.center();
	  double r_startangle = startangle*3.1415927/180/16;
	  double dx = cos(r_startangle) * radius;
	  double dy = sin(r_startangle) * radius;

	  fprintf(fp, "<path fill=\"none\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" d=\"M%lg,%lg a%d,%d 0 0,1 %lg,%lg\" />\n",
		  ce.x() - dx, ce.y() + dy,
		  radius, radius,
		  dx+dx, -dy-dy);
	}
      }
      break;
    case UmlProvided:
      p.drawPixmap(QPoint((int) x(), (int) y()), *providedPixmap);
      if (fp != 0)
	fprintf(fp, "<ellipse fill=\"none\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\" />\n",
		// 4 is the margin inside the pixmap
		(int) (x() + PROVIDED_RADIUS + 4),
		(int) (y() + PROVIDED_RADIUS + 4),
		(int) PROVIDED_RADIUS, (int) PROVIDED_RADIUS);
      break;
    default:
      // to avoid compiler warning
      break;
    }
  }
  
  if (selected())
    show_mark(p, rect());
}

UmlCode ArrowJunctionCanvas::type() const {
  return UmlArrowJunction;
}

void ArrowJunctionCanvas::remove_line(ArrowCanvas * l, bool onbrkjoin) {
  DiagramItem::remove_line(l, onbrkjoin);
  if (!onbrkjoin && lines.isEmpty())
    delete_it();
}

void ArrowJunctionCanvas::open() {
}

void ArrowJunctionCanvas::menu(const QPoint&) {
#if 0
  if (lines.at(0)->may_join()) {
    QPopupMenu m;
    
    m.insertItem(new MenuTitle(TR("Line break"), m.font()), -1);
    m.insertSeparator();
    m.insertItem(TR("Remove from diagram"), 0);
    
    switch (m.exec(QCursor::pos())) {
    case 0:
      // removes the point replacing the lines around it by a single line
      lines.at(0)->join(lines.at(1), this);
      break;
    default:
      return;
    }
    
    package_modified();
  }
#endif
}

QString ArrowJunctionCanvas::may_start(UmlCode &) const {
  return TR("illegal");
}

QString ArrowJunctionCanvas::may_connect(UmlCode &, const DiagramItem *) const {
  return TR("illegal");
}

bool ArrowJunctionCanvas::alignable() const {
  return TRUE;
}

void ArrowJunctionCanvas::save(QTextStream & st, bool, QString & warning) const {
  nl_indent(st);
  st << "arrowjunctioncanvas " << get_ident() << ' ';
  interface->save(st, TRUE, warning);
  nl_indent(st);
  save_xyz(st, this, "  xyz");
  save_xy(st, label, " label_xy");
}

ArrowJunctionCanvas * ArrowJunctionCanvas::read(char * & st, UmlCanvas * canvas,
						char * k)
{
  if (!strcmp(k, "arrowjunctioncanvas")) {
    int id = read_id(st);
    BrowserClass * br = BrowserClass::read_ref(st);
    ArrowJunctionCanvas * result = new ArrowJunctionCanvas(canvas, 0, 0, br, id);
    read_keyword(st, "xyz");
    read_xyz(st, result);
    result->set_center100();
    
    if (read_file_format() >= 27) {
      read_keyword(st, "label_xy");
      read_xy(st, result->label);
      result->label->set_center100();
    }
    
    result->show();
    
    return result; 
  }
  else
    return 0;
}

void ArrowJunctionCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  connect(interface->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(interface->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}

void ArrowJunctionCanvas::history_hide() {
  DiagramCanvas::setVisible(FALSE);
  unconnect();
}

