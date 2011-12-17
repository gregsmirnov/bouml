// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
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





#include <qworkspace.h>
#include <qnamespace.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qlabel.h>
#include <qcursor.h>




#include "BrowserDiagram.h"
#include "DiagramWindow.h"
#include "DiagramView.h"
#include "UmlWindow.h"
#include "UmlCanvas.h"
#include "myio.h"
#include "UmlPixmap.h"
#include "translate.h"

static QString zoomText() { return TR("Enter a new scale value or use the arrows to change the <i>scale</i>"); }
static QString fitzoomText() { return TR("Click this button to compute the <i>fit scale</i> allowing to try to show all the diagram"); }
static QString optwinsizeText() { return TR("Click this button to compute the smallest window size allowing to show all the diagram"); }
static QString editText() { return TR("Click this button to call the diagram menu like on a right mouse click"
				      " done out of any element"); }

DiagramWindow::DiagramWindow(BrowserDiagram * br, const QString & s)
    : QMainWindow(UmlWindow::get_workspace(), (const char *) s, WDestructiveClose),
      no_save(FALSE), browser_node(br) {
  setCaption(s);
  canvas = new UmlCanvas(br->get_format(), br);
  
  setFocusPolicy(QWidget::StrongFocus);
}

DiagramWindow::~DiagramWindow() {
  delete canvas;
}

void DiagramWindow::call_menu() {
  edit->setDown(FALSE);
  get_view()->menu(QCursor::pos());
}

void DiagramWindow::raise() {
  showNormal();
  setFocus();
}

void DiagramWindow::selectOn() {
  select->setOn(TRUE);
  hit_select();
}

void DiagramWindow::hit_select() {
  hit_button(UmlSelect, select);
}

void DiagramWindow::add_edit_button(QToolBar *toolbar) {
  edit = new QToolButton(*editButton, TR("edit"), QString::null,
			 this, SLOT(call_menu()), toolbar, "edit");
  QWhatsThis::add(edit, editText());
  
  edit->setToggleButton(FALSE);
}

void DiagramWindow::add_scale_cmd(QToolBar * toolbar) {
  sb_zoom = new QSpinBox(SCALE_MIN, SCALE_MAX, 10, toolbar, TR("scale"));
  //sb_zoom->setPrefix("scale ");
  sb_zoom->setSuffix("%");
  sb_zoom->setValue(100);
  connect(sb_zoom, SIGNAL(valueChanged(int)), this, SLOT(new_scale(int)));
  QWhatsThis::add(sb_zoom, zoomText());
  
  QToolButton * fitscale =
    new QToolButton(*fitscaleButton, TR("optimal scale"), QString::null,
		    this, SLOT(fit_scale()), toolbar, "optimal scale");
  QWhatsThis::add(fitscale, fitzoomText());
  
  //
  
  optwinsize = new QToolButton(*optwindowsizeButton, TR("optimal window size"), QString::null,
			 this, SLOT(optimal_window_size()), toolbar, "optimal window size");
  QWhatsThis::add(optwinsize, optwinsizeText());
  
  optwinsize->setToggleButton(FALSE);
}

void DiagramWindow::new_scale(int percent) {
  double np = ((double) percent)/100.0;
  
  if (canvas->zoom() != np) {
    get_view()->set_zoom(np);
    sb_zoom->setValue(percent);
  }
}

void DiagramWindow::fit_scale() {
  get_view()->fit_scale();
  sb_zoom->setValue((int) (canvas->zoom() * 100));
}

void DiagramWindow::optimal_window_size() {
  get_view()->do_optimal_window_size();
}

void DiagramWindow::change_zoom(int n) {
  int z = sb_zoom->value() + n;
  
  new_scale((z > 200) 
	    ? 200 
	    : ((z < 30) ? 30 : z));
}

void DiagramWindow::package_modified() const {
  browser_node->package_modified();
}

bool DiagramWindow::frozen() const {
  return (BrowserNode::edition_active() ||
	  !browser_node->is_writable());
}

void DiagramWindow::save_session(QTextStream & st) {
  // can't access to the window position, even through geometry
  QString warning;
  
  st << "  ";
  ((BrowserNode *) browser_node)->save(st, TRUE, warning);
  st << "\n    " << width() << ' ' << height() << ' ';
  get_view()->save_session(st);
}

void DiagramWindow::read_session(char * & st) {






  unsigned w = read_unsigned(st);
	
  resize(w, read_unsigned(st));

  new_scale(read_unsigned(st));
  get_view()->read_session(st);
}

void DiagramWindow::session_window_size() {



}

void DiagramWindow::save(const char * ext, QString & warning,
			 BooL & is_new) const {
  double zm  = canvas->zoom();
  
  if (zm != 1)
    get_view()->set_zoom(1);

  QString diagram_def;
  QTextOStream st(&diagram_def);
  int current_indent = indent();
  
  indent0();
  get_view()->save(st, warning, FALSE);
  save_definition(browser_node->get_ident(), ext, diagram_def, is_new);
  indent(current_indent);
  
  if (zm != 1)
    get_view()->set_zoom(zm);
}

void DiagramWindow::duplicate(int dest_id, const char * ext) const {
  QString warning;
  double zm  = canvas->zoom();
  
  if (zm != 1)
    get_view()->set_zoom(1);

  QString diagram_def;
  QTextOStream st(&diagram_def);
  int current_indent = indent();
  BooL is_new = TRUE;
  
  indent0();
  get_view()->save(st, warning, FALSE);
  save_definition(dest_id, ext, diagram_def, is_new);
  indent(current_indent);
  
  if (zm != 1)
    get_view()->set_zoom(zm);
}

QString DiagramWindow::copy_selected() const {
  QString warning;
  double zm  = canvas->zoom();
  
  if (zm != 1)
    get_view()->set_zoom(1);

  QString diagram_def;
  QTextOStream st(&diagram_def);
  
  indent0();
  get_view()->save(st, warning, TRUE);
  
  if (zm != 1)
    get_view()->set_zoom(zm);
  
  return diagram_def;
}

