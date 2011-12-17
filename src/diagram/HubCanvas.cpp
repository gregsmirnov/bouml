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

#include "HubCanvas.h"
#include "ArrowCanvas.h"
#include "UmlCanvas.h"
#include "myio.h"
#include "BrowserDiagram.h"
#include "MenuTitle.h"
#include "translate.h"

HubCanvas::HubCanvas(UmlCanvas * canvas, int x, int y, int id) 
    : DiagramCanvas(0, canvas, x, y, -HUB_SIZE, -HUB_SIZE, id) {
  browser_node = canvas->browser_diagram();
}

HubCanvas::~HubCanvas() {
}

void HubCanvas::delete_available(BooL &, BooL & out_model) const {
  out_model |= TRUE;
}

bool HubCanvas::alignable() const {
  return TRUE;
}

bool HubCanvas::copyable() const {
  return selected();
}

void HubCanvas::change_scale() {
  // the size is not modified
  QCanvasRectangle::setVisible(FALSE);
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

void HubCanvas::draw(QPainter & p) {
  if (! visible()) return;
  
  QRect r = rect();

  p.drawRect(r);

  FILE * fp = svg();

  if (fp != 0)
    fprintf(fp, "<g>\n"
	    "\t<rect fill=\"none\" stroke=\"black\" stroke-opacity=\"1\""
	    " x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" rx=\"10\" />\n"
	    "</g>\n",
	    r.left(), r.top(), r.width() - 1, r.height() - 1);
  
  if (selected())
    show_mark(p, r);
}

UmlCode HubCanvas::type() const {
  return UmlHub;
}

void HubCanvas::open() {
}

void HubCanvas::menu(const QPoint&) {
  QPopupMenu m;
  
  m.insertItem(new MenuTitle(TR("Network connexion"), m.font()), -1);
  m.insertSeparator();
  m.insertItem(TR("Remove from diagram"), 0);
  
  switch (m.exec(QCursor::pos())) {
  case 0:
    delete_it();
    break;
  default:
    return;
  }
  
  package_modified();
}

QString HubCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlAssociation:
  case UmlAnchor:
    return 0;
  default:
    return TR("illegal");
  }
}

QString HubCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  if (l == UmlAnchor)
    return dest->may_start(l);
  
  switch (dest->type()) {
  case UmlDeploymentNode:
  case UmlHub:
    return (l == UmlAssociation) ? 0 : TR("illegal");
  default:
    return TR("illegal");
  }
}

void HubCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref)
    st << "hubcanvas_ref " << get_ident();
  else {
    nl_indent(st);
    st << "hubcanvas " << get_ident();
    save_xyz(st, this, " xyz");
  }
}

HubCanvas * HubCanvas::read(char * & st, UmlCanvas * canvas,
			    char * k)
{
  if (!strcmp(k, "hubcanvas_ref"))
    return ((HubCanvas *) dict_get(read_id(st), "hubcanvas", canvas));
  if (strcmp(k, "hubcanvas"))
    return 0;
  
  int id = read_id(st);
  
  read_keyword(st, "xyz");
  
  int x = (int) read_double(st);
  
  HubCanvas * result =
    new HubCanvas(canvas, x, (int) read_double(st), id);
  
  result->setZ(read_double(st));
  result->show();
  
  return result;
}

