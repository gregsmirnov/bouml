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
#include <qpainter.h>

#include "LabelCanvas.h"
#include "DiagramCanvas.h"
#include "myio.h"
#include "translate.h"

const char * LabelCanvas::Triangle = "^^^";
const char * LabelCanvas::Zigzag = "~~~";

LabelCanvas::LabelCanvas(const QString & n, UmlCanvas * canvas, int x, int y,
			 bool bold, bool italic, bool underlined, bool mlcentered)
    : QCanvasText(n, canvas), DiagramItem(-1, canvas) {
  if (bold)
    setFont((italic) ? ((UmlCanvas *) canvas)->get_font(UmlNormalBoldItalicFont)
		     : ((UmlCanvas *) canvas)->get_font(UmlNormalBoldFont));
  else if (italic)
    setFont(((UmlCanvas *) canvas)->get_font(UmlNormalItalicFont));
  else
    setFont((underlined) ? ((UmlCanvas *) canvas)->get_font(UmlNormalUnderlinedFont)
			 : ((UmlCanvas *) canvas)->get_font(UmlNormalFont));
  multi_lines_centered = mlcentered;
  setX(x);
  setY(y);
  setZ(LABEL_Z);
  set_center100();  
  if (canvas->paste())
    canvas->select(this);
}

LabelCanvas::~LabelCanvas() {
}

void LabelCanvas::delete_it() {
  ((UmlCanvas *) canvas())->del(this);
}

UmlCanvas * LabelCanvas::the_canvas() const {
  return ((UmlCanvas *) canvas());
}

void LabelCanvas::set_name(const QString & s) {
  hide();
  
  setText(s);
  if (font().strikeOut())
    setFont(((UmlCanvas *) canvas())->get_font(UmlNormalFont));

  show();
  canvas()->update();
}

void LabelCanvas::set_strikeout(bool yes) {
  if (yes) {
    if (!font().strikeOut())
      setFont(((UmlCanvas *) canvas())->get_font(UmlNormalStrikeOutFont));
  }
  else if (font().strikeOut())
    setFont(((UmlCanvas *) canvas())->get_font(UmlNormalFont));
}

void LabelCanvas::set_underlined(bool yes) {
  if (yes) {
    if (!font().underline())
      setFont(((UmlCanvas *) canvas())->get_font(UmlNormalUnderlinedFont));
  }
  else if (font().underline())
    setFont(((UmlCanvas *) canvas())->get_font(UmlNormalFont));
}

void LabelCanvas::change_scale() {
  hide();

  if (font().bold())
    setFont((font().italic()) ? ((UmlCanvas *) canvas())->get_font(UmlNormalBoldItalicFont)
			      : ((UmlCanvas *) canvas())->get_font(UmlNormalBoldFont));
  else if (font().italic())
    setFont(((UmlCanvas *) canvas())->get_font(UmlNormalItalicFont));
  else if (font().underline())
    setFont(((UmlCanvas *) canvas())->get_font(UmlNormalUnderlinedFont));
  else
    setFont((font().strikeOut()) ? ((UmlCanvas *) canvas())->get_font(UmlNormalStrikeOutFont)
				 : ((UmlCanvas *) canvas())->get_font(UmlNormalFont));
  recenter();
  
  show();
}

void LabelCanvas::recenter() {
  double scale = the_canvas()->zoom();
  QPoint c = center();
  
  QCanvasText::moveBy(((int) (center_x_scale100 * scale + 0.5)) - c.x(),
		      ((int) (center_y_scale100 * scale + 0.5)) - c.y());
}

void LabelCanvas::set_center100() {
  QPoint c = center();
  double scale = the_canvas()->zoom();
    
  center_x_scale100  = (int) (c.x()/scale + 0.5);
  center_y_scale100  = (int) (c.y()/scale + 0.5);
}

void LabelCanvas::moveBy(double dx, double dy) {
  QCanvasText::moveBy(dx, dy);
  if (! the_canvas()->do_zoom())
    set_center100();
}


void LabelCanvas::move_outside(QRect r, double angle) {
  QPoint center(r.center());
  QRect lbl_r(0, 0, width(), height());
  
  // place it centered on r
  lbl_r.moveCenter(center);
  
  // goes outside r, moving by 5 points
  double cx = center.x();
  double cy = center.y();
  double d_x = 5*cos(angle * 3.1415927 / 180);
  double d_y = 5*sin(angle * 3.1415927 / 180);
  
  do {
    cx += d_x;
    cy += d_y;
    
    QPoint p((int) cx, (int) cy);
    
    lbl_r.moveCenter(p);
  } while (lbl_r.intersects(r));
  
  move(lbl_r.x(), lbl_r.y());
}

void LabelCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  p.setBackgroundMode(::Qt::TransparentMode);
  
  QRect r = boundingRect();
  FILE * fp = svg();
  int index = text().find(Triangle);

  if (fp != 0)
    fputs("<g>\n", fp);
  
  if (index == 0) {
    // triangle only
    int w = width() / 2 - 1;
    int b = r.bottom();
    int cx = (r.left() + r.right()) / 2;

    p.drawLine(cx - w, b, cx + w, b);
    p.lineTo(cx, (int) y());
    p.lineTo(cx - w, b);

    if (fp != 0)
      fprintf(fp, "\t<polygon fill=\"none\" stroke=\"black\" stroke-opacity=\"1\""
	      " points=\"%d,%d %d,%d %d,%d\" />\n",
	      cx - w, b, cx + w, b, cx, (int) y());
  }
  else if (index != -1) {
    // label then triangle under
    p.setFont(font());
    p.drawText(rect(), ::Qt::AlignHCenter, text().mid(0, index - 1));
    if (fp != 0)
      draw_text(rect(), ::Qt::AlignHCenter, text().mid(0, index - 1),
		p.font(), fp);

    QFontMetrics fm(font());
    int w = fm.width(Triangle) / 2 - 1;
    int cx = (r.left() + r.right()) / 2;
    int cy = (int) y() + fm.height();
    int b = r.bottom();
    
    p.drawLine(cx - w, b, cx + w, b);
    p.lineTo(cx, cy);
    p.lineTo(cx - w, b);

    if (fp != 0)
      fprintf(fp, "\t<polygon fill=\"none\" stroke=\"black\" stroke-opacity=\"1\""
	      " points=\"%d,%d %d,%d %d,%d\" />\n",
	      cx - w, b, cx + w, b, cx, cy);
  }
  else if (text() != Zigzag) {
    // no triangle nor zigzag
    if (text().find('\n') != -1) {
      p.setFont(font());
      
      int flg = (multi_lines_centered) ? ::Qt::AlignHCenter : 0;
      
      p.drawText(rect(), flg, text().mid(0, index - 1));
      if (fp != 0)
	draw_text(rect(), flg, text().mid(0, index - 1),
		  p.font(), fp);
    }
    else {
      QCanvasText::draw(p);
      if (fp != 0)
	draw_text(rect(), 0, text(), p.font(), fp);
    }
  }
  else {
    // zigzag (only used as stereotype)
    int t = r.top() + 2;
    int h = height() - 2;
    int yb = r.bottom() - h / 4;
    int xr = r.right() - r.width() / 4;
    
    p.drawLine(r.left(), t, r.right(), t);
    p.lineTo(r.left(), yb);
    p.lineTo(r.right(), yb);
    p.lineTo(xr, r.bottom() - h / 2);
    p.drawLine(r.right(), yb, xr, r.bottom());

    if (fp != 0) {
      fprintf(fp, "\t<polyline fill=\"none\" stroke=\"black\" stroke-opacity=\"1\""
	      " points=\"%d,%d %d,%d %d,%d %d,%d %d,%d\" />\n",
	      r.left(), t, r.right(), t, r.left(), yb, r.right(), yb, xr, r.bottom() - h / 2);
      fprintf(fp, "\t<line stroke=\"black\" stroke-opacity=\"1\""
	      " x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
	      r.right(), yb, xr, r.bottom());
    }
  }
  
  if (fp != 0)
    fputs("</g>\n", fp);

  if (selected())
    show_mark(p, r);
}

UmlCode LabelCanvas::type() const {
  return UmlLabel;
}

int LabelCanvas::rtti() const {
  return RTTI_LABEL;
}

QPoint LabelCanvas::center() const {
  return boundingRect().center();
}

QRect LabelCanvas::rect() const {
  return boundingRect();
}

bool LabelCanvas::isSelected() const {
  return selected();
}

bool LabelCanvas::contains(int, int) const {
  return FALSE;
}

QString LabelCanvas::may_start(UmlCode &) const {
  return TR("illegal");
}

QString LabelCanvas::may_connect(UmlCode &, const DiagramItem *) const {
  return TR("illegal");
}

void LabelCanvas::connexion(UmlCode, DiagramItem *,
			    const QPoint &, const QPoint &) {
  // no connexion available
}

void LabelCanvas::open() {
}

void LabelCanvas::menu(const QPoint&) {
}

double LabelCanvas::get_z() const {
  return z();
}

void LabelCanvas::set_z(double z) {
  setZ(z);
}

//

void LabelCanvas::save(QTextStream  & st, bool, QString &) const {
  st << "label ";
  save_string(text(), st);
  if (font().bold())
    st << " bold";
  if (font().italic())
    st << " italic";
  if (font().underline())
    st << " underlined";
  save_xyz(st, this, " xyz");
}

LabelCanvas * LabelCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (strcmp(k, "label"))
    return 0;
  
  QString s = read_string(st);
  bool bold = FALSE;
  bool italic = FALSE;
  bool underlined = FALSE;
  
  for (;;) {
    k = read_keyword(st);
    
    if (!strcmp(k, "bold"))
      bold = TRUE;
    else if (!strcmp(k, "italic"))
      italic = TRUE;
    else if (!strcmp(k, "underlined"))
      underlined = TRUE;
    else
      break;
  }

  int x = (int) read_double(st);
  
  LabelCanvas * result =
    new LabelCanvas(s, canvas, x, (int) read_double(st),
		    bold, italic, underlined);
  
  if (!strcmp(k, "xyz"))
    result->setZ(read_double(st));
  else if (!strcmp(k, "xy"))
    // old version
    ;//result->setZ(OLD_LABEL_Z);
  else
    wrong_keyword(k, "xyz");
  
  return result;
}

void LabelCanvas::history_save(QBuffer & b) const {
  ::save(this, b);
  ::save(center_x_scale100, b);
  ::save(center_y_scale100, b);
  ::save(x(), b);
  ::save(y(), b);
  ::save(z(), b);
}

void LabelCanvas::history_load(QBuffer & b) {  
  ::load(center_x_scale100, b);
  ::load(center_y_scale100, b);
  double dx = load_double(b) - x();

  QCanvasText::moveBy(dx, load_double(b) - y());
  QCanvasText::setZ(load_double(b));
  QCanvasItem::setSelected(FALSE);
  QCanvasItem::setVisible(TRUE);
}

void LabelCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
}

void LabelCanvas::check_stereotypeproperties() {
  // do nothing
}
