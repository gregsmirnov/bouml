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

#include "ActivityNodeCanvas.h"
#include "ActivityContainerCanvas.h"
#include "FlowCanvas.h"
#include "SimpleData.h"
#include "BrowserActivityNode.h"
#include "LabelCanvas.h"
#include "UmlPixmap.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"
#include "translate.h"

#define MIN_FORK_JOIN_LARGESIDE 19
#define FORK_JOIN_SMALLSIDE 15

ActivityNodeCanvas::ActivityNodeCanvas(BrowserNode * bn, UmlCanvas * canvas,
				       int x, int y)
    : DiagramCanvas(0, canvas, x, y, 16, 16, 0),
      horiz(FALSE), manual_size(FALSE) {
  browser_node = bn;
  set_xpm();
  check_stereotypeproperties();
   
  connect(bn->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(bn->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));

  if (canvas->must_draw_all_relations())
    draw_all_flows();
}

ActivityNodeCanvas::ActivityNodeCanvas(UmlCanvas * canvas, int id)
    : DiagramCanvas(canvas, id), horiz(FALSE), manual_size(FALSE) {
  // for read operation
  browser_node = 0;
  xpm = 0;
}

ActivityNodeCanvas::~ActivityNodeCanvas() {
}

void ActivityNodeCanvas::delete_it() {
  disconnect(browser_node->get_data(), 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(modified()));
  
  DiagramCanvas::delete_it();
}

void ActivityNodeCanvas::deleted() {
  delete_it();
  canvas()->update();
}

void ActivityNodeCanvas::remove(bool from_model) {
  if (! from_model)
    delete_it();
  else
    browser_node->delete_it();	// will remove canvas
}

bool ActivityNodeCanvas::force_inside() {
  // if its activity is present, force inside it
  
  QCanvasItemList all = the_canvas()->allItems();
  QCanvasItemList::Iterator cit;
  BrowserNode * parent = (BrowserNode *) browser_node->parent();

  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
      
      if ((di != 0) &&
	  IsaActivityContainer(di->type()) &&
	  (((ActivityContainerCanvas *) di)->get_bn() == parent)) {
	((ActivityContainerCanvas *) di)->force_inside(this, this);
	return TRUE;
      }
    }
  }
  
  return FALSE;
}

void ActivityNodeCanvas::set_xpm() {
  bool big = the_canvas()->zoom() >= 1.0;

  switch (browser_node->get_type()) {
  case InitialAN:
    xpm = (big) ? initialBigPixmap : initialPixmap;
    break;
  case ActivityFinalAN:
    xpm = (big) ? finalBigPixmap : finalPixmap;
    break;
  case FlowFinalAN:
    xpm = (big) ? exitPointBigPixmap : exitPointPixmap;
    break;
  case DecisionAN:
  case MergeAN:
    xpm = (the_canvas()->shadow() == 0)
      ? ((big) ? choiceBigPixmap : choicePixmap)
      : ((big) ? choiceShadowBigPixmap : choiceShadowPixmap);
    break;
  case ForkAN:
  case JoinAN:
    if (!manual_size) {
      xpm = (horiz)
	? ((big) ? joinForkHorizBigPixmap : joinForkHorizPixmap)
	: ((big) ? joinForkBigPixmap : joinForkPixmap);
      break;
    }
    // no break
  default:
    xpm = 0;
    return;
  }

  setSize(xpm->width(), xpm->height());
}

void ActivityNodeCanvas::change_scale() {
  QCanvasRectangle::setVisible(FALSE);
  
  if (manual_size) {
    double scale = the_canvas()->zoom();
    
    if (horiz) {
      int w = (int) (width_scale100*scale);
      
      if (w < MIN_FORK_JOIN_LARGESIDE)
	w = MIN_FORK_JOIN_LARGESIDE;
      
      setSize(w | 1, FORK_JOIN_SMALLSIDE);
    }
    else {
      int h = (int) (height_scale100*scale);
      
      if (h < MIN_FORK_JOIN_LARGESIDE)
	h = MIN_FORK_JOIN_LARGESIDE;
      
      setSize(FORK_JOIN_SMALLSIDE, h | 1);
    }
    
    DiagramCanvas::resize(width(), height());
    recenter();
  }
  else
    set_xpm();
  
  recenter();
  QCanvasRectangle::setVisible(TRUE);
}

aCorner ActivityNodeCanvas::on_resize_point(const QPoint & p) {
  switch (browser_node->get_type()) {
  case ForkAN:
  case JoinAN:
    return ::on_resize_point(p, rect());
  default:
    return NoCorner;
  }
}

void ActivityNodeCanvas::resize(aCorner c, int dx, int dy, QPoint & o) {
  switch (browser_node->get_type()) {
  case ForkAN:
  case JoinAN:
    manual_size = TRUE;
    xpm = 0;
    
    if (horiz)
      DiagramCanvas::resize(c, dx, 0, o, MIN_FORK_JOIN_LARGESIDE, FORK_JOIN_SMALLSIDE, TRUE);
    else
      DiagramCanvas::resize(c, 0, dy, o, FORK_JOIN_SMALLSIDE, MIN_FORK_JOIN_LARGESIDE, TRUE);
    break;
  default:
    break;
  }
}

void ActivityNodeCanvas::resize(const QSize & sz, bool w, bool h) {
  switch (browser_node->get_type()) {
  case ForkAN:
  case JoinAN:
    manual_size = TRUE;
    xpm = 0;
    
    if (horiz)
      DiagramCanvas::resize(sz, w, h, MIN_FORK_JOIN_LARGESIDE, FORK_JOIN_SMALLSIDE, TRUE);
    else
      DiagramCanvas::resize(sz, w, h, FORK_JOIN_SMALLSIDE, MIN_FORK_JOIN_LARGESIDE, TRUE);
    break;
  default:
    break;
  }
}

void ActivityNodeCanvas::modified() {
  hide();
  hide_lines();
  switch (browser_node->get_type()) {
  case DecisionAN:
  case MergeAN:
    set_xpm();
    break;
  default:
    break;
  }
  show();
  update_show_lines();
  force_self_rel_visible();
  check_stereotypeproperties();
  if (the_canvas()->must_draw_all_relations())
    draw_all_flows();
  canvas()->update();
  if (label != 0)
    label->set_name(browser_node->get_name());
  package_modified();
}

void ActivityNodeCanvas::post_loaded() {
  force_self_rel_visible();
  if (the_canvas()->must_draw_all_relations())
    draw_all_flows();
}

void ActivityNodeCanvas::draw(QPainter & p) {
  if (!visible() || ((xpm == 0) && !manual_size)) return;

  QRect r = rect();
  QRect intern_r;
  
  p.setBackgroundMode(::Qt::OpaqueMode);
  
  if (xpm != 0)
    p.drawPixmap(r.topLeft(), *xpm);
  else {
    // jork join manually sized
    if (horiz) {
      intern_r.setX(r.x() + 1);
      intern_r.setWidth(r.width() - 2);
      intern_r.setY(r.y() + 6);
      intern_r.setHeight(3);
    }
    else {
      intern_r.setX(r.x() + 6);
      intern_r.setWidth(3);
      intern_r.setY(r.y() + 1);
      intern_r.setHeight(r.height() - 2);
    }
    p.fillRect(intern_r, ::Qt::black);
  }
    
  if (selected())
    show_mark(p, r);

  FILE * fp = svg();

  if (fp != 0) {
    bool big = the_canvas()->zoom() >= 1.0;
    int px = (int) x();
    int py = (int) y();

    switch (browser_node->get_type()) {
    case InitialAN:
      if (big)
	fprintf(fp, "\t<ellipse fill=\"black\" stroke=\"none\" cx=\"%d\" cy=\"%d\" rx=\"8.5\" ry=\"8.5\" />\n",
		px + 9, py + 9);
      else
	fprintf(fp, "\t<ellipse fill=\"black\" stroke=\"none\" cx=\"%d\" cy=\"%d\" rx=\"5.5\" ry=\"5.5\" />\n",
		px + 7, py + 7);
      break;
    case ActivityFinalAN:
      if (big) {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"11.5\" ry=\"11.5\" />\n",
		px + 12, py + 12);
	fprintf(fp, "\t<ellipse fill=\"black\" cx=\"%d\" cy=\"%d\" rx=\"8.5\" ry=\"8.5\" />\n"
		"</g>\n",
		px + 12, py + 12);
      }
      else {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"7.5\" ry=\"7.5\" />\n",
		px + 8, py + 8);
	fprintf(fp, "\t<ellipse fill=\"black\" cx=\"%d\" cy=\"%d\" rx=\"4.5\" ry=\"4.5\" />\n"
		"</g>\n",
		px + 8, py + 8);
      }
      break;
    case FlowFinalAN:
      if (big) {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"8.5\" ry=\"8.5\" />\n",
		px + 9, py + 9);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 4, py + 4, px + 14, py + 14);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 14, py + 4, px + 4, py + 14);
      }
      else {
	fprintf(fp, "<g>\n"
		"\t<ellipse fill=\"white\" stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" cx=\"%d\" cy=\"%d\" rx=\"5.5\" ry=\"5.5\" />\n",
		px + 7, py + 7);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		px + 4, py + 4, px + 10, py + 10);
	fprintf(fp, "\t<line stroke=\"black\" stroke-width=\"1\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n"
		"</g>\n",
		px + 10, py + 4, px + 4, py + 10);
      }
      break;
    case DecisionAN:
    case MergeAN:
      // note : shadow not produced
      if (big)
	fprintf(fp, "<polygon fill=\"white\" stroke=\"black\" stroke-opacity=\"1\" points =\"%d,%d %d,%d %d,%d %d,%d\" />\n",
		px + 3, py + 18, px + 12, py + 1, px + 21, py + 18, px + 12, py + 35);
      else
	fprintf(fp, "<polygon fill=\"white\" stroke=\"black\" stroke-opacity=\"1\" points =\"%d,%d %d,%d %d,%d %d,%d\" />\n",
		px + 2, py + 12, px + 8, py + 1, px + 14, py + 12, px + 8, py + 23);
      break;
    case ForkAN:
    case JoinAN:
      if (horiz) {
	if (manual_size)
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  intern_r.x(), intern_r.y(), intern_r.x() + intern_r.width() - 1, intern_r.y());
	else if (big)
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  px + 1, py + 7, px + 23, py + 7);
	else
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  px + 1, py + 7, px + 16, py + 7);
      }
      else {
	if (manual_size)
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  intern_r.x(), intern_r.y(), intern_r.x(), intern_r.y() + intern_r.height() - 1);
	else if (big)
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  px + 6, py + 1, px + 6, py + 24);
	else
	  fprintf(fp, "<line stroke=\"black\" stroke-width=\"3\" stroke-opacity=\"1\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
		  px + 6, py + 1, px + 6, py + 17);
      }
      break;
    default:
      break;
    }
  }
}

UmlCode ActivityNodeCanvas::type() const {
  return browser_node->get_type();
}

void ActivityNodeCanvas::delete_available(BooL & in_model, BooL & out_model) const {
  out_model |= TRUE;
  in_model |= browser_node->is_writable();
}

bool ActivityNodeCanvas::alignable() const {
  return TRUE;
}

bool ActivityNodeCanvas::copyable() const {
  return selected();
}

void ActivityNodeCanvas::open() {
  browser_node->open(TRUE);
}

void ActivityNodeCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu toolm(0);
  int index;
  
  m.insertItem(new MenuTitle(browser_node->get_data()->definition(FALSE, TRUE), m.font()), -1);
  m.insertSeparator();
  m.insertItem(TR("Upper"), 0);
  m.insertItem(TR("Lower"), 1);
  m.insertItem(TR("Go up"), 13);
  m.insertItem(TR("Go down"), 14);
  m.insertSeparator();
  switch (browser_node->get_type()) {
  case ForkAN:
  case JoinAN:
    m.insertItem((horiz) ? TR("draw vertically") : TR("draw horizontally"), 2);
    m.insertSeparator();
    break;
  default:
    break;
  }
  /*m.insertItem("Edit drawing settings", 2);
  m.insertSeparator();*/
  m.insertItem(TR("Edit activity node"), 3);
  m.insertSeparator();
  m.insertItem(TR("Select in browser"), 4);
  if (linked())
    m.insertItem(TR("Select linked items"), 5);
  m.insertSeparator();
  /*if (browser_node->is_writable())
    if (browser_node->get_associated() !=
	(BrowserNode *) the_canvas()->browser_diagram())
      m.insertItem("Set associated diagram",6);
  m.insertSeparator();*/
  m.insertItem(TR("Remove from diagram"), 7);
  if (browser_node->is_writable())
    m.insertItem(TR("Delete from model"), 8);
  m.insertSeparator();
  if (Tool::menu_insert(&toolm, browser_node->get_type(), 20))
    m.insertItem(TR("Tool"), &toolm);
  
  switch (index = m.exec(QCursor::pos())) {
  case 0:
    upper();
    modified();	// call package_modified()
    return;
  case 1:
    lower();
    modified();	// call package_modified()
    return;
  case 13:
    z_up();
    modified();	// call package_modified()
    return;
  case 14:
    z_down();
    modified();	// call package_modified()
    return;
  case 2:
    horiz ^= TRUE;
    if (!manual_size)
      set_xpm();
    else {
      setSize(height(), width());
      DiagramCanvas::resize(width(), height());
    }
    modified();	// call package_modified()
    return;
  case 3:
    browser_node->open(TRUE);
    return;
  case 4:
    browser_node->select_in_browser();
    return;
  case 5:
    the_canvas()->unselect_all();
    select_associated();
    return;
  case 7:
    //remove from diagram
    delete_it();
    break;
  case 8:
    //delete from model
    browser_node->delete_it();	// will delete the canvas
    break;
  default:
    if (index >= 20)
      ToolCom::run(Tool::command(index - 20), browser_node);
    return;
  }
  
  package_modified();
}

void ActivityNodeCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") {
    browser_node->select_in_browser();
    return;
  }
  else if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Go up")
    z_up();
  else if (s == "Go down")
    z_down();
  else {
    browser_node->apply_shortcut(s);
    return;
  }

  modified();
  package_modified();
}

QString ActivityNodeCanvas::may_start(UmlCode & l) const {
  switch (l) {
  case UmlFlow:
    return ((BrowserActivityNode *) browser_node)->may_start();
  case UmlDependOn:
    return TR("illegal");
  default: // anchor
    return 0;
  }
}

QString ActivityNodeCanvas::may_connect(UmlCode & l, const DiagramItem * dest) const {
  if (l == UmlAnchor)
    return dest->may_start(l);
  else if(dest->get_bn() == 0)
    return TR("illegal");
  else
    return ((BrowserActivityNode *) browser_node)->may_connect(dest->get_bn());
}

void ActivityNodeCanvas::connexion(UmlCode action, DiagramItem * dest,
				   const QPoint &, const QPoint &) {
  ArrowCanvas * a;
  
  if (action == UmlFlow)
    a = new FlowCanvas(the_canvas(), this, dest, 0, 0, -1.0, -1.0);
  else
    a = new ArrowCanvas(the_canvas(), this, dest, action, 0, FALSE, -1.0, -1.0);
  
  a->show();
  the_canvas()->select(a);
}

void ActivityNodeCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref) {
    st << "activitynodecanvas_ref " << get_ident() << " // "
      << browser_node->full_name();
  }
  else {
    nl_indent(st);
    st << "activitynodecanvas " << get_ident() << ' ';
    browser_node->save(st, TRUE, warning);
    indent(+1);
    nl_indent(st);
    if (horiz)
      st << "horizontal ";
    if (manual_size)
      save_xyzwh(st, this, " xyzwh");
    else
      save_xyz(st, this, "xyz");
    save_stereotype_property(st, warning);
    indent(-1);
    nl_indent(st);
    st << "end";
  }
}

ActivityNodeCanvas * ActivityNodeCanvas::read(char * & st, UmlCanvas * canvas,
					      char * k)
{
  if (!strcmp(k, "activitynodecanvas_ref"))
    return ((ActivityNodeCanvas *) dict_get(read_id(st), "ActivityNodeCanvas", canvas));
  else if (!strcmp(k, "activitynodecanvas")) {
    int id = read_id(st);
    BrowserActivityNode * ps = BrowserActivityNode::read_ref(st);
    ActivityNodeCanvas * result = new ActivityNodeCanvas(canvas, id);
    
    result->browser_node = ps;
    connect(ps->get_data(), SIGNAL(changed()), result, SLOT(modified()));
    connect(ps->get_data(), SIGNAL(deleted()), result, SLOT(deleted()));

    switch (result->browser_node->get_type()) {
    case DecisionAN:
    case MergeAN:
      connect(DrawingSettings::instance(), SIGNAL(changed()), result, SLOT(modified()));
      break;
    default:
      break;
    }

    k = read_keyword(st);
    
    if (!strcmp(k, "horizontal")) {
      result->horiz = TRUE;
      k = read_keyword(st);
    }
    
    if (!strcmp(k, "xyzwh")) {
      read_xyzwh(st, result);
      result->manual_size = TRUE;
    }
    else if (strcmp(k, "xyz"))
      wrong_keyword(k, "xyz");
    else
      read_xyz(st, result);

    if (read_file_format() >= 58) {
      k = read_keyword(st);
      result->read_stereotype_property(st, k);	// updates k
      
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    result->set_xpm();
    if (result->manual_size) {
      result->width_scale100 = result->width();
      result->height_scale100 = result->height();
    }
    result->set_center100();
    result->show();
    result->check_stereotypeproperties();
    
    if (canvas->paste())
      result->remove_if_already_present();
    
    return result;
  }
  else 
    return 0;
}

void ActivityNodeCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  disconnect(browser_node->get_data(), 0, this, 0);
}

void ActivityNodeCanvas::history_save(QBuffer & b) const {
  DiagramCanvas::history_save(b);
  ::save((int) manual_size, b);
  if (manual_size) {
    ::save(width_scale100, b);
    ::save(height_scale100, b);
    ::save(width(), b);
    ::save(height(), b);
  }
}

void ActivityNodeCanvas::history_load(QBuffer & b) {
  DiagramCanvas::history_load(b);
  
  int ms;
  
  ::load(ms, b);
  manual_size = (ms != 0);
  
  if (manual_size) {
    ::load(width_scale100, b);
    ::load(height_scale100, b);
    
    int w, h;
    
    ::load(w, b);
    ::load(h, b);
    QCanvasRectangle::setSize(w, h);
  }
  
  connect(browser_node->get_data(), SIGNAL(changed()), this, SLOT(modified()));
  connect(browser_node->get_data(), SIGNAL(deleted()), this, SLOT(deleted()));
}
