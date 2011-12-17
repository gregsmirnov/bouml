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

#include "TextCanvas.h"
#include "MLEDialog.h"
#include "UmlGlobal.h"
#include "UmlCanvas.h"
#include "myio.h"
#include "MenuTitle.h"
#include "BrowserDiagram.h"
#include "strutil.h"
#include "DialogUtil.h"
#include "SettingsDialog.h"
#include "ToolCom.h"
#include "translate.h"

TextCanvas::TextCanvas(UmlCanvas * canvas, int x, int y, int id)
    : DiagramCanvas(0, canvas, x, y, TEXT_CANVAS_MIN_SIZE,
		    TEXT_CANVAS_MIN_SIZE, id),
      itsfont(UmlNormalFont), fg_c(UmlBlack), bg_c(UmlTransparent) {
  browser_node = canvas->browser_diagram();
}

TextCanvas::~TextCanvas() {
}

void TextCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QColor bgcolor = p.backgroundColor();
  QPen fgcolor = p.pen();
  
  if (bg_c == UmlTransparent)
    p.setBackgroundMode(::Qt::TransparentMode);
  else {
    p.setBackgroundMode(::Qt::OpaqueMode);
    p.setBackgroundColor(color(bg_c));
  }
  
  if (fg_c != UmlTransparent)
    p.setPen(color(fg_c));
  
  QRect r = rect();
  
  p.setFont(the_canvas()->get_font(itsfont));
  p.drawText (r.left(), r.top(), r.width(), r.height(), 
	      ::Qt::AlignLeft + ::Qt::AlignTop + ::Qt::WordBreak, text);
  
  FILE * fp = svg();
  
  if (fp != 0)
    draw_text (r.left(), r.top(), r.width(), r.height(), 
	       ::Qt::AlignLeft + ::Qt::AlignTop + ::Qt::WordBreak,
	       text, p.font(), fp, fg_c, bg_c);
  
  p.setFont(the_canvas()->get_font(UmlNormalFont));
  p.setBackgroundColor(bgcolor);
  p.setPen(fgcolor);
  
  if (selected())
    show_mark(p, r);
}

UmlCode TextCanvas::type() const {
  return UmlText;
}

void TextCanvas::open() {
  static QSize sz;
  
  // warning : 'note' is already unicode
  QString s = fromUnicode(text);
  bool created = s.isEmpty();
  
  MLEDialog::get(s, QCursor::pos(), sz);
  
  if (s.isEmpty())
    delete_it();
  else {
    text = toUnicode(s);
    
    if (created) {  
      QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
      QSize sz = fm.size(0, text);
      
      DiagramCanvas::resize(sz.width(), sz.height());
    }
  
    // force son reaffichage
    hide();
    show();
  }
  canvas()->update();
  
  package_modified();
}

void TextCanvas::delete_available(BooL &, BooL & out_model) const {
  out_model |= TRUE;
}

void TextCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu fontsubm(0);
  
  m.insertItem(new MenuTitle(TR("Text"), m.font()), -1);
  m.insertSeparator();
  m.insertItem(TR("Upper"), 0);
  m.insertItem(TR("Lower"), 1);
  m.insertItem(TR("Go up"), 6);
  m.insertItem(TR("Go down"), 7);
  m.insertSeparator();
  m.insertItem(TR("Edit"), 2);
  m.insertSeparator();
  m.insertItem(TR("Color"), 5);
  m.insertItem(TR("Font"), &fontsubm);  
  init_font_menu(fontsubm, the_canvas(), 10);
  if (linked()) {
    m.insertSeparator();
    m.insertItem(TR("Select linked items"), 3);
  }
  m.insertSeparator();
  m.insertItem(TR("Remove from diagram"),4);

  int index = m.exec(QCursor::pos());
  
  switch (index) {
  case 0:
    upper();
    // force son reaffichage
    hide();
    show();
    canvas()->update();
    break;
  case 1:
    lower();
    // force son reaffichage
    hide();
    show();
    canvas()->update();
    break;
  case 6:
    z_up();
    // force son reaffichage
    hide();
    show();
    canvas()->update();
    break;
  case 7:
    z_down();
    // force son reaffichage
    hide();
    show();
    canvas()->update();
    break;
  case 2:
    open();
    return;	// call package_modified
  case 3:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 4:
    // delete
    the_canvas()->del(this);
    break;
  case 5:
    for (;;) {
      ColorSpecVector co(2);
      
      co[0].set(TR("foreground"), &fg_c);
      co[1].set(TR("background"), &bg_c);
      
      SettingsDialog dialog(0, &co, TRUE, FALSE,
			    TR("Text color dialog"));
      
      dialog.raise();
      if (dialog.exec() != QDialog::Accepted)
	return;
      
      // force son reaffichage
      hide();
      show();
      canvas()->update();
      
      if (!dialog.redo())
	break;
      else
	package_modified();
    }
    break;
  default:
    if (index >= 10) {
      itsfont = (UmlFont) (index - 10);
      // force son reaffichage
      hide();
      show();
      canvas()->update();
      break;
    }
    else
      return;
  }
  
  package_modified();
}

void TextCanvas::apply_shortcut(QString s) {
  if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Go up")
    z_up();
  else if (s == "Go down")
    z_down();
  else if (s == "Edit") {
    open();  // call package_modified
    return;
  }
  else 
    return;

  // force son reaffichage
  hide();
  show();
  canvas()->update();
  
  package_modified();
}

QString TextCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : TR("illegal");
}

QString TextCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  return (l == UmlAnchor) ? dest->may_start(l) : TR("illegal");
}

bool TextCanvas::alignable() const {
  return TRUE;
}

bool TextCanvas::copyable() const {
  return selected();
}

bool TextCanvas::move_with(UmlCode k) const {
  return (k == UmlFragment) || DiagramCanvas::move_with(k);
}

aCorner TextCanvas::on_resize_point(const QPoint & p) {
  return ::on_resize_point(p, rect());
}

void TextCanvas::resize(aCorner c, int dx, int dy, QPoint & o) {
  int min = (int) (TEXT_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  DiagramCanvas::resize(c, dx, dy, o, min, min);
}

void TextCanvas::resize(const QSize & sz, bool w, bool h) {
  int min = (int) (TEXT_CANVAS_MIN_SIZE * the_canvas()->zoom());
  
  DiagramCanvas::resize(sz, w, h, min, min);
}

void TextCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref)
    st << "textcanvas_ref " << get_ident();
  else {
    nl_indent(st);
    st << "textcanvas " << get_ident() << ' ';
    save_string(fromUnicode(text), st);
    nl_indent(st);
    if (itsfont != UmlNormalFont)
      st << "  font " << stringify(itsfont);
    if (fg_c != UmlBlack)
      st << "  fg " << stringify(fg_c);
    if (bg_c != UmlTransparent)
      st << "  bg " << stringify(bg_c);
    save_xyzwh(st, this, "  xyzwh");
  }
}

TextCanvas * TextCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "textcanvas_ref"))
    return ((TextCanvas *) dict_get(read_id(st), "textcanvas", canvas));
  else if (!strcmp(k, "textcanvas")) {
    int id = read_id(st);
    QString text = toUnicode(read_string(st));
    UmlFont font;
    UmlColor fg_c = UmlBlack;
    UmlColor bg_c = UmlTransparent;

    k = read_keyword(st);
    read_font(st, "font", font, k);
    read_color(st, "fg", fg_c, k);
    read_color(st, "bg", bg_c, k);
    
    if (strcmp(k, "xyzwh"))
      wrong_keyword(k, "xyzwh");
    
    int x = (int) read_double(st);
    
    TextCanvas * result =
      new TextCanvas(canvas, x, (int) read_double(st), id);
    
    read_zwh(st, result);
    result->width_scale100 = result->width();
    result->height_scale100 = result->height();
    result->set_center100();
    result->text = text;
    result->itsfont = font;
    result->fg_c = fg_c;
    result->bg_c = bg_c;
    result->show();
    
    return result;
  }
  else
    return 0;
}

void TextCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save(width_scale100, b);
  ::save(height_scale100, b);
  ::save(width(), b);
  ::save(height(), b);
}

void TextCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  ::load(width_scale100, b);
  ::load(height_scale100, b);
  
  int w, h;
  
  ::load(w, b);
  ::load(h, b);
  QCanvasRectangle::setSize(w, h);
}

// for plug outs

void TextCanvas::send(ToolCom * com, QCanvasItemList & all)
{
  QCanvasItemList::Iterator cit;

  for (cit = all.begin(); cit != all.end(); ++cit) {
    DiagramItem *di = QCanvasItemToDiagramItem(*cit);
    
    if ((di != 0) && 
	(*cit)->visible() &&
	(di->type() == UmlText)) {
      TextCanvas * tx = (TextCanvas *) di;
      QCString s = fromUnicode(tx->text);
      
      com->write_bool(TRUE);	// one more
      com->write_string((const char *) s);
      com->write(tx->rect());
    }
  }
  
  com->write_bool(FALSE);	// done
}
