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





#include <qcursor.h>
#include <qpainter.h>
#include <qpopupmenu.h> 

#include "ImageCanvas.h"
#include "Images.h"
#include "ImageDialog.h"
#include "myio.h"
#include "MenuTitle.h"
#include "strutil.h"
#include "translate.h"

ImageCanvas::ImageCanvas(UmlCanvas * canvas, int x, int y, QString pa, int id)
    : DiagramCanvas(0, canvas, x, y, 30, 30, id), path(pa) {
  if ((px = get_pixmap(path)) != 0) {
    width_scale100 = px->width();
    height_scale100 = px->height();
    
    px = get_pixmap(path, canvas->zoom());
    setSize(px->width(), px->height());
    set_center100();
  }
}

ImageCanvas::~ImageCanvas() {
}

void ImageCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  if (px == 0)
    p.fillRect (rect(), ::Qt::darkGray);
  else
    p.drawPixmap((int) x(), (int) y(), *px);
  
  FILE * fp = svg();
  
  if (fp != 0)
    // pixmap not really exported in SVG
    fprintf(fp, "<rect fill=\"%s\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\""
	    " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" />\n",
	    svg_color(UmlBlack), (int) x(), (int) y(), width() - 1, height() - 1);
  
  if (selected())
    show_mark(p, rect());
}

UmlCode ImageCanvas::type() const {
  return UmlImage;
}

void ImageCanvas::delete_available(BooL &, BooL & out_model) const {
  out_model |= TRUE;
}

bool ImageCanvas::alignable() const {
  return TRUE;
}

bool ImageCanvas::copyable() const {
  return selected();
}

void ImageCanvas::open() {
  QString s = path;
  ImageDialog d(s);
  
  if ((d.exec() == QDialog::Accepted) &&
      !s.isEmpty() &&
      (s != path)) {
    QPixmap * p = get_pixmap(s);
    
    if (p != 0) {
      path = s;
      width_scale100 = p->width();
      height_scale100 = p->height();      
      px = get_pixmap(path, the_canvas()->zoom());
      
      hide();
      hide_lines();
      setSize(px->width(), px->height());
      recenter();
      show();
      update_show_lines();
      canvas()->update();
      package_modified();
    }
  }
}

ImageCanvas * ImageCanvas::add(UmlCanvas * canvas, int x, int y)
{
  QString s;
  ImageDialog d(s);
  
  if ((d.exec() == QDialog::Accepted) && !s.isEmpty()) {
    QPixmap * p = get_pixmap(s, canvas->zoom());
    
    if (p != 0)
      return new ImageCanvas(canvas, x, y, s, 0);
  }

  return 0;
}

void ImageCanvas::modified() {
  // force son reaffichage
  hide();
  show();
  canvas()->update();
  package_modified();
}

void ImageCanvas::change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  px = get_pixmap(path, the_canvas()->zoom());
  setSize(px->width(), px->height());
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void ImageCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu fontsubm(0);
  
  m.insertItem(new MenuTitle(TR("Image"), m.font()), -1);
  m.insertSeparator();
  m.insertItem(TR("Upper"), 0);
  m.insertItem(TR("Lower"), 1);
  m.insertItem(TR("Go up"), 2);
  m.insertItem(TR("Go down"), 3);
  m.insertSeparator();
  m.insertItem(TR("Edit"), 4);
  if (linked()) {
    m.insertSeparator();
    m.insertItem(TR("Select linked items"), 5);
  }
  m.insertSeparator();
  m.insertItem(TR("Remove from diagram"), 6);

  int index = m.exec(QCursor::pos());
  
  switch (index) {
  case 0:
    upper();
    modified();	// call package_modified()
    return;
  case 1:
    lower();
    modified();	// call package_modified()
    return;
  case 2:
    z_up();
    modified();	// call package_modified()
    return;
  case 3:
    z_down();
    modified();	// call package_modified()
    return;
  case 4:
    open();
    // all done by open
    return;
  case 5:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 6:
    delete_it();
    break;
  }
  
  package_modified();
}

void ImageCanvas::apply_shortcut(QString s) {
  if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Go up")
    z_up();
  else if (s == "Go down")
    z_down();
  else if (s == "Edit") {
    open();  // call modified then package_modified
    return;
  }
  else 
    return;

  modified();
}

QString ImageCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : TR("illegal");
}

QString ImageCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  return (l == UmlAnchor) ? dest->may_start(l) : TR("illegal");
}

void ImageCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref) {
    st << "image_ref " << get_ident();
  }
  else {
    nl_indent(st);
    
    st << "image " << get_ident() << ' ';
    save_string(fromUnicode(path), st);
    save_xyz(st, this, " xyz");
    st << " end";
  }
}

ImageCanvas * ImageCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "image_ref"))
    return (ImageCanvas *) dict_get(read_id(st), "image", canvas);
  else if (!strcmp(k, "image")) {
    int id = read_id(st);
    QString pa = toUnicode(read_string(st));
    ImageCanvas * result = new ImageCanvas(canvas, 0, 0, pa, id);
    
    read_keyword(st, "xyz");
    read_xyz(st, result);
    read_keyword(st, "end");
    
    result->set_center100();
    result->show();
    
    return result;
  }
  else
    return 0;
}

void ImageCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
}

void ImageCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void ImageCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
}
