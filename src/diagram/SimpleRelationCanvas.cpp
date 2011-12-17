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
#include <math.h>
#include <qpopupmenu.h>

#include "SimpleRelationCanvas.h"
#include "ArrowPointCanvas.h"
#include "UmlCanvas.h"
#include "LabelCanvas.h"
#include "BrowserSimpleRelation.h"
#include "BrowserClass.h"
#include "SimpleRelationData.h"
#include "StereotypePropertiesCanvas.h"
#include "BrowserDiagram.h"
#include "UmlWindow.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "DialogUtil.h"
#include "strutil.h"

SimpleRelationCanvas::SimpleRelationCanvas(UmlCanvas * canvas,
					   DiagramItem * b, DiagramItem * e,
					   BrowserNode * bb, UmlCode t, int id,
					   float d_start, float d_end,
					   SimpleRelationData * d)
      : ArrowCanvas(canvas, b, e, t, id, TRUE, d_start, d_end),
        br_begin(bb), data(d), stereotypeproperties(0) {
  if ((e->type() != UmlArrowPoint) && (bb == 0)) {
    // end of line construction
    update_begin(e);
  }
  else if (d != 0) {
    if (e->type() != UmlArrowPoint)
      check_stereotypeproperties();
    connect(d, SIGNAL(changed()), this, SLOT(modified()));
    connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  }
  
  // manages the case start == end
  if (b == e)
    cut_self();
  
  if (d != 0)
    update(TRUE);
}

SimpleRelationCanvas::~SimpleRelationCanvas() {
}

void SimpleRelationCanvas::delete_it() {
  if (stereotypeproperties != 0)
    ((UmlCanvas *) canvas())->del(stereotypeproperties);
  
  ArrowCanvas::delete_it(); // call unconnect
}

void SimpleRelationCanvas::unconnect() {
  disconnect(data, 0, this, 0);  
  
  ArrowCanvas::unconnect();
}

void SimpleRelationCanvas::deleted() {
  delete_it();
  canvas()->update();
  package_modified();
}

void SimpleRelationCanvas::delete_available(BooL & in_model, BooL & out_model) const {
  out_model |= TRUE;
  in_model |= data->get_start()->is_writable();
}

void SimpleRelationCanvas::remove(bool from_model) {
  if (! from_model) {
    if (the_canvas()->must_draw_all_relations()) {
      const SimpleRelationCanvas * a = this;
  
      while (a->begin->type() == UmlArrowPoint) {
	a = (SimpleRelationCanvas *) ((ArrowPointCanvas *) a->begin)->get_other(a);
	if (a == 0)
	  break;
      }

      if (a && !a->begin->isSelected() && !a->begin->get_bn()->deletedp()) {
	a = this;
  
	while (a->end->type() == UmlArrowPoint) {
	  a = (SimpleRelationCanvas *) ((ArrowPointCanvas *) a->end)->get_other(a);
	  if (a == 0)
	    break;
	}
  
	if (a && !a->end->isSelected() && !a->end->get_bn()->deletedp()) {
	  msg_warning("Bouml", "<i>Draw all relations</i> forced to <i>no</i>");
	  the_canvas()->dont_draw_all_relations();
	}
      }
    }

    delete_it();
  }
  else
    data->delete_it();	// will remove canvas
}


// Updates br_begin going back, Returns the start browser node

BrowserNode * SimpleRelationCanvas::update_begin(DiagramItem * cnend) {
  // static to be updated in all the cases
  static SimpleRelationData * d = 0;
  
  if (begin->type() == UmlArrowPoint) {
    SimpleRelationCanvas * other =
      ((SimpleRelationCanvas *) ((ArrowPointCanvas *) begin)->get_other(this));
    
    br_begin = other->update_begin(cnend);	// update d
  }
  else {
    // the relation start here
    br_begin = begin->get_bn();
    d = (SimpleRelationData *) begin->add_relation(type(), cnend);
  }
  
  connect(d, SIGNAL(changed()), this, SLOT(modified()));
  connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  data = d;
  
  return br_begin;
}

void SimpleRelationCanvas::open() {
  if (data &&
      !data->get_start()->in_edition())	// for dble click on diagram
    data->edit();
}

void SimpleRelationCanvas::menu(const QPoint &) {
  if ((data != 0) && !data->get_start()->in_edition()) {
    SimpleRelationCanvas * pstereotype;
    
    {
      ArrowCanvas * aplabel;	// cannot exist
      ArrowCanvas * apstereotype;
      
      search_supports(aplabel, apstereotype);
      pstereotype = (SimpleRelationCanvas *) apstereotype;
    }
    
    QPopupMenu m(0);
    QPopupMenu geo(0);
    QPopupMenu toolm(0);
    
    m.insertItem(new MenuTitle(data->definition(FALSE, TRUE), m.font()), -1);
    m.insertSeparator();
    m.insertItem("Edit", 0);
    m.insertSeparator();
    
    m.insertItem("Select in browser", 2);
    if (pstereotype) {
      m.insertSeparator();
      m.insertItem("Select stereotype", 3);
      m.insertItem("Stereotype default position", 4);
      if (stereotype == 0)
	m.insertItem("Attach stereotype to this segment", 6);
    }
  
    if (get_start() != get_end()) {
      m.insertSeparator();
      init_geometry_menu(geo, 10);
      m.insertItem("Geometry (Ctrl+l)", &geo);
    }
    
    m.insertSeparator();
    m.insertItem("Remove from diagram",7);
    if (data->get_start()->is_writable())
      m.insertItem("Delete from model", 8);
    
    m.insertSeparator();
    if (Tool::menu_insert(&toolm, itstype, 20))
      m.insertItem("Tool", &toolm);
    
    int rank = m.exec(QCursor::pos());
    
    switch (rank) {
    case 0:
      data->edit();
      return;
    case 2:
      data->get_start()->select_in_browser();
      return;
    case 3:
      the_canvas()->unselect_all();
      the_canvas()->select(pstereotype->stereotype);
      return;
    case 4:
      pstereotype->stereotype_default_position();
      return;
    case 6:
      stereotype = pstereotype->stereotype;
      pstereotype->stereotype = 0;
      stereotype_default_position();
      break;
    case 7:
      // not removed from the browser : just hide it
      remove(FALSE);
      break;
    case 8:
      data->delete_it();	// will delete the canvas
      break;
    default:
      if (rank >= 20) {
	ToolCom::run(Tool::command(rank - 20), data->get_start());
	return;
      }
      else if (rank >= 10) {
	rank -= 10;
	if (rank == RecenterBegin)
	  set_decenter(-1.0, decenter_end);
	else if (rank == RecenterEnd)
	  set_decenter(decenter_begin, -1.0);
	else if (rank != (int) geometry)
	  set_geometry((LineGeometry) rank, TRUE);
	else
	  return;
      }
      else
	return;
    }
    
    package_modified();
  }
}

void SimpleRelationCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser")
    data->get_start()->select_in_browser();
  else
    data->get_start()->apply_shortcut(s);
}

ArrowPointCanvas * SimpleRelationCanvas::brk(const QPoint & p) {
  ArrowPointCanvas * ap =
    new ArrowPointCanvas(the_canvas(), p.x(), p.y());
  
  ap->setZ(z() + 1);
  
  SimpleRelationCanvas * other =
    // do not give data to not call update()
    new SimpleRelationCanvas(the_canvas(), ap, end, br_begin,
			     itstype, 0, decenter_begin, decenter_end);
  
  other->data = data;
  connect(data, SIGNAL(changed()), other, SLOT(modified()));
  connect(data, SIGNAL(deleted()), other, SLOT(deleted()));

  ap->add_line(this);
  end->remove_line(this, TRUE);
  end = ap;
  
  if ((p - beginp).manhattanLength() < (p - endp).manhattanLength()) {
    if (stereotype != 0) {
      other->stereotype = stereotype;
      stereotype = 0;
    }
  }
  
  ap->show();
  other->show();
  
  modified();		// to set/unset name, role_b and multiplicity_b
  other->modified();	// to set/unset role_a and multiplicity_a
  
  return ap;
}


ArrowCanvas * SimpleRelationCanvas::join(ArrowCanvas * other, ArrowPointCanvas * ap) {
  // has already check is join is possible (self relation must have two points)
  ArrowCanvas * result = ArrowCanvas::join(other, ap);

  ((SimpleRelationCanvas *) result)->modified();
  
  return result;
}

void SimpleRelationCanvas::modified() {
  if (visible()) {
    hide();
    update(TRUE);
    show();
    if (begin->type() != UmlArrowPoint)
      check_stereotypeproperties();
    canvas()->update();
    package_modified();
  }
}

void SimpleRelationCanvas::setSelected(bool yes) {
  UmlWindow::set_commented((yes) ? data->get_start() : 0);

  QCanvasPolygon::setSelected(yes);
}

void SimpleRelationCanvas::update(bool updatepos) {
  if (data) {
    itstype = data->get_type();
    
    QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
    SimpleRelationCanvas * pstereotype;
    
    {
      ArrowCanvas * aplabel;	// cannot exist
      ArrowCanvas * apstereotype;
      
      search_supports(aplabel, apstereotype);
      pstereotype = (SimpleRelationCanvas *) apstereotype;
    }
    
    // manages relation's stereotype
    
    QString s = data->get_short_stereotype();
    
    if (s.isEmpty()) {
      // relation does not have stereotype
      if (pstereotype != 0) {
	// removes it
	the_canvas()->del(pstereotype->stereotype);
	pstereotype->stereotype = 0;
      }
    }
    else {
      s = toUnicode(s);

      if (s[0] != '{')
	s = QString("<<") + s + ">>";
      
      if ((pstereotype == 0) && (begin->type() != UmlArrowPoint)) {
	// adds relation's stereotype
	stereotype = new LabelCanvas(s, the_canvas(), 0, 0);
	stereotype_default_position();
      }
      else if ((pstereotype != 0) && 
	       (pstereotype->stereotype->get_name() != s)) {
	// update name
	pstereotype->stereotype->set_name(s);
	pstereotype->stereotype_default_position();
      }
    }
  }
  
  if (updatepos)
    update_pos();
}

void SimpleRelationCanvas::stereotype_default_position() const {
  default_stereotype_position();
  package_modified();
}
	
BasicData * SimpleRelationCanvas::get_data() const {
  return data;
}

void SimpleRelationCanvas::drop(BrowserNode * bn, UmlCanvas * canvas)
{
  SimpleRelationData * def = (SimpleRelationData *) bn->get_data();
  BrowserNode * from = def->get_start_node();
  BrowserNode * to = def->get_end_node();
  DiagramItem * ccfrom = 0;
  DiagramItem * ccto = 0;
  QCanvasItemList all = canvas->allItems();
  QCanvasItemList::Iterator cit;

  // the two classes are drawn ?
  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible()) {
      DiagramItem * adi = QCanvasItemToDiagramItem(*cit);
    
      if (adi != 0) {
	if (adi->get_bn() == from) {
	  ccfrom = adi;
	  if (ccto != 0)
	    break;
	}
	if (adi->get_bn() == to) {
	  ccto = adi;
	  if (ccfrom != 0)
	    break;
	}
      }
    }
  }
  
  if ((ccfrom != 0) && (ccto != 0)) {
    if (ccfrom->has_relation(def))
      msg_information("Bouml", "relation already drawn");
    else {
      SimpleRelationCanvas * rel = 
	new SimpleRelationCanvas(canvas, ccfrom, ccto, from,
				 bn->get_type(), 0, -1.0, -1.0, def);
      
      rel->show();
      rel->package_modified();
    }
  }
}

// the relation is not yet drawn, 
void SimpleRelationCanvas::drop(BrowserNode * bn, UmlCanvas * canvas,
				QPtrDict<DiagramItem> & drawn)
{
  SimpleRelationData * def = (SimpleRelationData *) bn->get_data();
  BrowserNode * from = def->get_start_node();
  BrowserNode * to = def->get_end_node();
  DiagramItem * ccfrom = drawn[from->get_data()];
  DiagramItem * ccto = drawn[to->get_data()];
  
  if ((ccfrom != 0) && (ccto != 0)) {
    SimpleRelationCanvas * rel = 
      new SimpleRelationCanvas(canvas, ccfrom, ccto, from,
			       bn->get_type(), 0, -1.0, -1.0, def);
    
    rel->show();
    rel->package_modified();
    
    drawn.replace(def, rel);
      
    // package set modified by caller
  }
}

//

void SimpleRelationCanvas::setVisible(bool yes) {
  ArrowCanvas::setVisible(yes);

  if (stereotypeproperties)
    stereotypeproperties->setVisible(yes);
}

void SimpleRelationCanvas::moveBy(double dx, double dy) {
  ArrowCanvas::moveBy(dx, dy);

  if ((stereotypeproperties != 0) && !stereotypeproperties->selected())
    stereotypeproperties->moveBy(dx, dy);
}

void SimpleRelationCanvas::select_associated() {
  if (!selected()) {
    if ((stereotypeproperties != 0) && !stereotypeproperties->selected())
      the_canvas()->select(stereotypeproperties);
    ArrowCanvas::select_associated();
  }
}

void SimpleRelationCanvas::check_stereotypeproperties() {
  // the note is memorized by the first segment
  if (begin->type() == UmlArrowPoint)
    ((SimpleRelationCanvas *) ((ArrowPointCanvas *) begin)->get_other(this))
      ->check_stereotypeproperties();
  else {
    QString s = data->get_start()->stereotypes_properties();
    
    if (!s.isEmpty() &&
	the_canvas()->browser_diagram()->get_show_stereotype_properties())
      StereotypePropertiesCanvas::needed(the_canvas(), this, s,
					 stereotypeproperties, center());
    else if (stereotypeproperties != 0) {
      stereotypeproperties->delete_it();
      stereotypeproperties = 0;
    }
  }
}

bool SimpleRelationCanvas::represents(BrowserNode * bn) {
  return (data == bn->get_data());
}

//

void SimpleRelationCanvas::save(QTextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "simplerelationcanvas_ref " << get_ident();
  else if (begin->type() != UmlArrowPoint) {
    // relation canvas start
    nl_indent(st);
    st << "simplerelationcanvas " << get_ident() << ' ';
    data->get_start()->save(st, TRUE, warning);	// data doesn't have id
    indent(+1);
    if (geometry != NoGeometry) {
      nl_indent(st);
      st << "geometry " << stringify(geometry);
      if (!fixed_geometry)
	st << " unfixed";
    }
    if (decenter_begin >= 0) {
      // float output/input bugged
      nl_indent(st);
      st << "decenter_begin " << ((int) (decenter_begin * 1000));
    }
    if (decenter_end >= 0) {
      // float output/input bugged
      nl_indent(st);
      st << "decenter_end " << ((int) (decenter_end * 1000));
    }
    (const SimpleRelationCanvas *) ArrowCanvas::save_lines(st, TRUE, TRUE, warning);
    
    if (stereotypeproperties != 0)
      stereotypeproperties->save(st, FALSE, warning);
    
    indent(-1);
    nl_indent(st);
    st << "end";    
  }
}

SimpleRelationCanvas * SimpleRelationCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "simplerelationcanvas_ref"))
    return ((SimpleRelationCanvas *) dict_get(read_id(st), "simplerelationcanvas", canvas));
  else if (!strcmp(k, "simplerelationcanvas")) {    
    int id = read_id(st);
    BrowserSimpleRelation * br = BrowserSimpleRelation::read_ref(st);
    SimpleRelationData * rd = (SimpleRelationData *) br->get_data();
    LineGeometry geo;
    bool fixed;
    
    k = read_keyword(st);
    if (! strcmp(k, "geometry")) {
      geo = line_geometry(read_keyword(st));
      k = read_keyword(st);
      if (! strcmp(k, "unfixed")) {
	k = read_keyword(st);
	fixed = FALSE;
      }
      else
	fixed = TRUE;
    }
    else {
      geo = NoGeometry;
      fixed = FALSE;
    }
    
    float dbegin;
    float dend;

    if (! strcmp(k, "decenter_begin")) {
      dbegin = read_double(st) / 1000;
      k = read_keyword(st);
    }
    else
      dbegin = -1;

    if (! strcmp(k, "decenter_end")) {
      dend = read_double(st) / 1000;
      k = read_keyword(st);
    }
    else
      dend = -1;
      
    unread_keyword(k, st);
    
    read_keyword(st, "from");
    read_keyword(st, "ref");
    
    DiagramItem * bi = dict_get(read_id(st), "canvas", canvas);
    BrowserNode * b = rd->get_start_node();
    UmlCode t = rd->get_type();
    SimpleRelationCanvas * first = 0;
    SimpleRelationCanvas * result;
    LabelCanvas * stereotype;
    double z;
              
    for (;;) {
      read_keyword(st, "z");
      z = read_double(st);
      k = read_keyword(st);
      
      if (!strcmp(k, "stereotype")) {
	(void) read_string(st);
	read_keyword(st, "xyz");
	
	int x = (int) read_double(st);
	int y = (int) read_double(st);
	
	if ((rd == 0) || (*(rd->get_stereotype()) == 0)) {
	  stereotype = 0;
	  read_double(st);	// z
	}
	else {
	  QString s = toUnicode(rd->get_short_stereotype());
	  
	  if (s[0] != '{')
	    s = QString("<<") + s + ">>";

	  stereotype = new LabelCanvas(s, canvas, x, y);
	  stereotype->setZ(read_double(st));
	}
	k = read_keyword(st);
      }
      else
	stereotype = 0;

      if (strcmp(k, "to"))
	wrong_keyword(k, "to");

      DiagramItem * di;
      
      if (strcmp(k = read_keyword(st), "ref")) {
	di = ArrowPointCanvas::read(st, canvas, k);
	
	if (di == 0)
	  unknown_keyword(k);
      }
      else
	di = dict_get(read_id(st), "classcanvas", canvas);

      // do not give rd to not call update()
      result = new SimpleRelationCanvas(canvas, bi, di, b, t, id, dbegin, dend);
      result->geometry = geo;
      result->fixed_geometry = fixed;
      result->set_z(z);
      result->data = rd;
      connect(rd, SIGNAL(changed()), result, SLOT(modified()));
      connect(rd, SIGNAL(deleted()), result, SLOT(deleted()));

      if (first == 0) {
	first = result;
	if (read_file_format() == 30)
	  // to remove redondant relation made by release 2.22
	  RelsToCheck.append(result);
      }
      if (stereotype != 0)
	(result->stereotype = stereotype)->show();
      result->show();
      
      if (di->type() != UmlArrowPoint)
	break;
      
      bi = di;
      
      read_keyword(st, "line");
      id = read_id(st);
    }

    if (read_file_format() >= 58) {
      // stereotype property
      
      k = read_keyword(st);
      
      first->stereotypeproperties = 
	StereotypePropertiesCanvas::read(st,  canvas, k, first);
      
      if (first->stereotypeproperties != 0)
	k = read_keyword(st);
      
      if (strcmp(k, "end"))
	wrong_keyword(k, "end");
    }
    
    // to add stereotype ... if needed
    
    first->update(FALSE);
    if (first != result)
      result->update(FALSE);
    
    first->check_stereotypeproperties();

    // manage case where the relation is deleted but present in the browser
    if (result->data->get_start()->deletedp())
      RelsToDel.append(result);
    else
      result->update_geometry();
    
    return result;
  }
  else
    return 0;
}

void SimpleRelationCanvas::history_hide() {
  QCanvasItem::setVisible(FALSE);
  unconnect();
}

void SimpleRelationCanvas::history_load(QBuffer & b) {
  ArrowCanvas::history_load(b);
  connect(data, SIGNAL(changed()), this, SLOT(modified()));
  connect(data, SIGNAL(deleted()), this, SLOT(deleted()));
}
