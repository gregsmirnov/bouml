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





#include <qpopupmenu.h> 
#include <qcursor.h>
#include <qpainter.h>

#include "SdLostFoundMsgSupportCanvas.h"
#include "SdMsgCanvas.h"
#include "UmlCanvas.h"
#include "BrowserDiagram.h"
#include "myio.h"
#include "translate.h"

SdLostFoundMsgSupportCanvas::SdLostFoundMsgSupportCanvas(UmlCanvas * canvas, int x, int y, int id) 
    : SdMsgSupport(canvas, x, y, -LOSTFOUND_SIZE, -LOSTFOUND_SIZE, id), msg(0) {
  browser_node = canvas->browser_diagram();
}

SdLostFoundMsgSupportCanvas::~SdLostFoundMsgSupportCanvas() {
}

void SdLostFoundMsgSupportCanvas::delete_available(BooL &, BooL & out_model) const {
  out_model |= TRUE;
}

void SdLostFoundMsgSupportCanvas::delete_it() {
  if (msg != 0)
    msg->delete_it();	// will update msgs
}

bool SdLostFoundMsgSupportCanvas::alignable() const {
  return TRUE;
}

bool SdLostFoundMsgSupportCanvas::copyable() const {
  return selected();
}

void SdLostFoundMsgSupportCanvas::change_scale() {
  // the size is not modified
  QCanvasRectangle::setVisible(FALSE);
  recenter();
  if (msg != 0)
    msg->update_hpos();
  QCanvasRectangle::setVisible(TRUE);
}

void SdLostFoundMsgSupportCanvas::set_z(double newz) {
  DiagramCanvas::set_z((msg != 0) ? msg->z() : newz);
}

void SdLostFoundMsgSupportCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();

  if (selected())
    p.fillRect(r, ::Qt::black);
  else {
    QBrush brsh = p.brush();
    
    p.setBrush(::Qt::black);
    p.drawEllipse(r.left(), r.top(), LOSTFOUND_SIZE, LOSTFOUND_SIZE);
    p.setBrush(brsh);
  }

  FILE * fp = svg();

  if (fp != 0) {
    const float rr = LOSTFOUND_SIZE / (float) 2.0;
    
    fprintf(fp, "<ellipse fill=\"black\" stroke=\"none\" cx=\"%g\" cy=\"%g\" rx=\"%g\" ry=\"%g\" />\n",
	    (float)(r.left() + rr), (float)(r.top() + rr), rr, rr);
  }
  
  // don't use show_mark is selected : too small
}

UmlCode SdLostFoundMsgSupportCanvas::type() const {
  return UmlLostFoundMsgSupport;
}

void SdLostFoundMsgSupportCanvas::open() {
}

void SdLostFoundMsgSupportCanvas::menu(const QPoint&) {
}

QString SdLostFoundMsgSupportCanvas::may_start(UmlCode & l) const {
  return (l == UmlAnchor) ? 0 : TR("illegal");
}

QString SdLostFoundMsgSupportCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  return  (l == UmlAnchor)
    ? dest->may_start(l)
    : TR("illegal");
}

void SdLostFoundMsgSupportCanvas::add(SdMsgBaseCanvas * m) {
  msg = m;
}

void SdLostFoundMsgSupportCanvas::remove(SdMsgBaseCanvas *) {
  DiagramCanvas::delete_it();
}

double SdLostFoundMsgSupportCanvas::min_y() const {
  return 1e10;
  /*
  return (msg == 0)
    ? 0
    : ((msg->get_dest() == this)
       ? ((SdMsgCanvas *) msg)->get_start()
       : msg->get_dest())
      ->min_y();
      */
}

void SdLostFoundMsgSupportCanvas::update_v_to_contain(const QRect re) {
  if (msg != 0)
    DiagramCanvas::moveBy(0, (msg->y() + msg->height() - 1)/2 - (y() + LOSTFOUND_SIZE - 1)/2);
  else
    // normally not possible
    DiagramCanvas::moveBy(0, (re.y() + re.height() - 1)/2 - (y() + LOSTFOUND_SIZE - 1)/2);
}

void SdLostFoundMsgSupportCanvas::moveBy(double dx, double dy) {
  if (msg != 0) {
    DiagramCanvas::moveBy(dx, (msg->y() + msg->height() - 1)/2 - (y() + LOSTFOUND_SIZE - 1)/2);
    msg->update_hpos();
  }
  else
    DiagramCanvas::moveBy(dx, dy);
}

bool SdLostFoundMsgSupportCanvas::isaDuration() const {
  return FALSE;
}

bool SdLostFoundMsgSupportCanvas::isOverlappingDuration() const {
  return FALSE;
}

void SdLostFoundMsgSupportCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref)
    st << "lostfoundmsgsupport_ref " << get_ident();
  else {
    nl_indent(st);
    st << "lostfoundmsgsupport " << get_ident();
    save_xyz(st, this, " xyz");
  }
}

SdLostFoundMsgSupportCanvas * SdLostFoundMsgSupportCanvas::read(char * & st,
								UmlCanvas * canvas,
								char * k)
{
  if (!strcmp(k, "lostfoundmsgsupport_ref"))
    return ((SdLostFoundMsgSupportCanvas *) dict_get(read_id(st), "lostfoundmsgsupport", canvas));
  if (strcmp(k, "lostfoundmsgsupport"))
    return 0;
  
  int id = read_id(st);
  
  read_keyword(st, "xyz");
  
  int x = (int) read_double(st);
  
  SdLostFoundMsgSupportCanvas * result =
    new SdLostFoundMsgSupportCanvas(canvas, x, (int) read_double(st), id);
  
  result->setZ(read_double(st));
  result->show();
  
  return result;
}

