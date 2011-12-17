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

#include "StereotypePropertiesCanvas.h"
#include "Settings.h"
#include "myio.h"
#include "MenuTitle.h"
#include "DialogUtil.h"
#include "SettingsDialog.h"
#include "ArrowCanvas.h"
#include "translate.h"

StereotypePropertiesCanvas::StereotypePropertiesCanvas(UmlCanvas * canvas,
						       DiagramItem * it,
						       int x, int y, int id)
    : NoteCanvas(canvas, x, y, id), di(it) {
  // for read
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(update()));
}

StereotypePropertiesCanvas::StereotypePropertiesCanvas(UmlCanvas * canvas,
						       DiagramItem * it,
						       QString s)
    : NoteCanvas(canvas, 0, 0, 0), di(it) {
  connect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(update()));
  
  note = s;	// already unicode
  
  QFontMetrics fm(the_canvas()->get_font(itsfont));
  QSize sz = fm.size(0, s);
  const int margin = (int) (2 * NOTE_MARGIN * the_canvas()->zoom());
  int w = sz.width() + margin;
  int h = sz.height() + margin;

  if (w < width())
    w = width();
  if (h < height())
    h = height();

  DiagramCanvas::resize(w, h);
  setZ(it->get_z());

  width_scale100 = w;
  height_scale100 = h;
  set_center100();
}

StereotypePropertiesCanvas::~StereotypePropertiesCanvas() {
}

void StereotypePropertiesCanvas::delete_it() {
  disconnect(DrawingSettings::instance(), SIGNAL(changed()), this, SLOT(update()));
  
  NoteCanvas::delete_it();
}

UmlCode StereotypePropertiesCanvas::type() const {
  return UmlStereotypeProperties;
}

void StereotypePropertiesCanvas::delete_available(BooL & in_model,
						  BooL & out_model) const {
  if (di->isSelected())
    di->delete_available(in_model, out_model);
}

bool StereotypePropertiesCanvas::copyable() const {
  return di->isSelected();
}

void StereotypePropertiesCanvas::open() {
  di->open();
}

void StereotypePropertiesCanvas::menu(const QPoint&) {
  QPopupMenu m(0);
  QPopupMenu fontsubm(0);
  
  m.insertItem(new MenuTitle(TR("Stereotype Properties"), m.font()), -1);
  m.insertSeparator();
  m.insertItem(TR("Upper"), 0);
  m.insertItem(TR("Lower"), 1);
  m.insertItem(TR("Go up"), 5);
  m.insertItem(TR("Go down"), 6);
  m.insertSeparator();
  m.insertItem(TR("Edit"), 2);
  m.insertSeparator();
  m.insertItem(TR("Font"), &fontsubm);  
  init_font_menu(fontsubm, the_canvas(), 10);
  m.insertItem(TR("Edit drawing settings"), 3);
  if (linked()) {
    m.insertSeparator();
    m.insertItem(TR("Select linked items"), 4);
  }
  m.insertSeparator();

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
  case 5:
    z_up();
    modified();	// call package_modified()
    return;
  case 6:
    z_down();
    modified();	// call package_modified()
    return;
  case 2:
    open();
    return;
  case 3:
    for (;;) {
      ColorSpecVector co(1);
      
      co[0].set(TR("note color"), &itscolor);

      SettingsDialog dialog(0, &co, FALSE);
      
      dialog.raise();
      if (dialog.exec() == QDialog::Accepted)
	modified();
      if (!dialog.redo())
	return;
      else
	package_modified();
    }
    break;
  case 4:
    the_canvas()->unselect_all();
    select_associated();
    return;
  default:
    if (index >= 10) {
      itsfont = (UmlFont) (index - 10);
      modified();	// call package_modified()
    }
    return;
  }
  
  package_modified();
}

void StereotypePropertiesCanvas::apply_shortcut(QString s) {
  if (s == "Upper")
    upper();
  else if (s == "Lower")
    lower();
  else if (s == "Go up")
    z_up();
  else if (s == "Go down")
    z_down();
  else
    return;

  modified();
  package_modified();
}

bool StereotypePropertiesCanvas::has_drawing_settings() const {
  return TRUE;
}

void StereotypePropertiesCanvas::edit_drawing_settings(QList<DiagramItem> & l) {
  for (;;) {
    ColorSpecVector co(1);
    UmlColor itscolor;
    
    co[0].set(TR("note color"), &itscolor);
    
    SettingsDialog dialog(0, &co, FALSE, TRUE);
    
    dialog.raise();
    if ((dialog.exec() == QDialog::Accepted) && !co[0].name.isEmpty()) {
      QListIterator<DiagramItem> it(l);
      
      for (; it.current(); ++it) {
	((StereotypePropertiesCanvas *) it.current())->itscolor = itscolor;
	((StereotypePropertiesCanvas *) it.current())->modified();	// call package_modified()
      }
    }
    if (!dialog.redo())
      break;
  }
}

void StereotypePropertiesCanvas::same_drawing_settings(QList<DiagramItem> & l) {
  QListIterator<DiagramItem> it(l);
  
  StereotypePropertiesCanvas * x = (StereotypePropertiesCanvas *) it.current();
  
  while (++it, it.current() != 0) {
    StereotypePropertiesCanvas * o =  (StereotypePropertiesCanvas *) it.current();
				 
    o->itscolor = x->itscolor;
    o->modified();	// call package_modified()
  }
}

void StereotypePropertiesCanvas::needed(UmlCanvas * canvas, DiagramItem * di, QString s,
					StereotypePropertiesCanvas *& sp, QPoint pt)
{
  if (sp == 0) {
    sp = new StereotypePropertiesCanvas(canvas, di, s);
    sp->move(pt.x(), pt.y());
    sp->show();
    sp->upper();
    (new ArrowCanvas(canvas, di, sp, UmlAnchor, 0, FALSE, -1.0, -1.0))->show();
  }
  else if (sp->note != s) {
    sp->note = s;
    sp->modified();	// call package_modified()
  }
}

void StereotypePropertiesCanvas::update() {
  di->check_stereotypeproperties();
}

void StereotypePropertiesCanvas::save(QTextStream & st, bool ref, QString &) const {
  if (ref) {
    st << "stereotypeproperties_ref " << get_ident();
  }
  else {
    nl_indent(st);
    
    st << "stereotypeproperties " << get_ident();
    indent(+1);
    nl_indent(st);
    save_internal(st);
    indent(-1);
  }
}

StereotypePropertiesCanvas *
  StereotypePropertiesCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "stereotypeproperties_ref"))
    return (StereotypePropertiesCanvas *) dict_get(read_id(st), "stereotypeproperties", canvas);
  else
    return 0;
}

StereotypePropertiesCanvas *
  StereotypePropertiesCanvas::read(char * & st, UmlCanvas * canvas, 
				   char * k, DiagramItem * it)
{
  if (!strcmp(k, "stereotypeproperties")) {
    int id = read_id(st);
    StereotypePropertiesCanvas * result =
      new StereotypePropertiesCanvas(canvas, it, 0, 0, id);
    
    result->read_internal(st);
    // update done by owner
    return result;
  }
  else
    return 0;
}

