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





#include <qapplication.h>
#include <qworkspace.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qlayout.h>

#include "UmlWindow.h"
#include "ClassDiagramWindow.h"
#include "ClassDiagramView.h"
#include "BrowserClassDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "translate.h"

QString addclassText() { return TR("Click this button to add a <i>class</i> in the diagram. <br><br>"
				   "You can also drop the class from the <b>browser</b>."); }
QString addpackageText() { return TR("Click this button to add a <i>package</i> in the diagram. <br><br>"
				     "You can also drop the package from the <b>browser</b>."); }
extern QString addfragmentText();
extern QString inheritText();
extern QString noteText();
extern QString anchorText();
QString dependencyText() { return TR("Click this button to create a <i>dependency</i>"); }
extern QString associationText();
QString directionalassociationextensionText() { return TR("Click this button to create an <i>unidirectional association</i> or an <i>extension</i>"); }
QString aggregationText() { return TR("Click this button to create an <i>aggregation</i>"); }
QString aggregationbyvalueText() { return TR("Click this button to create a <i>composition</i>"); }
QString directionalaggregationText() { return TR("Click this button to create a <i>directional aggregation</i>"); }
QString directionalaggregationbyvalueText() { return TR("Click this button to create a <i>directional composition</i>"); }
QString realizeText() { return TR("Click this button to create a <i>realization</i>"); }
extern QString textText();
extern QString imageText();

// id is an old ident in case of an import
ClassDiagramWindow::ClassDiagramWindow(const QString & s, BrowserClassDiagram * b, int id)
    : DiagramWindow(b, s), view(0) {
  QToolBar * toolbar = new QToolBar(this, "class diagram operations");
  addToolBar(toolbar, TR("Toolbar"), Top, TRUE);
  
  add_edit_button(toolbar);
  
  select =
    new QToolButton(*selectButton, TR("Select"), QString::null,
		    this, SLOT(hit_select()), toolbar, "select");
  select->setToggleButton(TRUE);
  select->setOn(TRUE);
  current_button = UmlSelect;
  
  addClass
    = new QToolButton(*classButton, TR("Add Class"), QString::null,
		      this, SLOT(hit_class()), toolbar, "add class");
  addClass->setToggleButton(TRUE);
  QWhatsThis::add(addClass, addclassText());
  
  addPackage
    = new QToolButton(*packageButton, TR("Add Package"), QString::null,
		      this, SLOT(hit_package()), toolbar, "add package");
  addPackage->setToggleButton(TRUE);
  QWhatsThis::add(addPackage, addpackageText());
  
  addFragment
    = new QToolButton(*fragmentButton, TR("Add Fragment"), QString::null,
		      this, SLOT(hit_fragment()), toolbar, "add fragment");
  addFragment->setToggleButton(TRUE);
  QWhatsThis::add(addFragment, addfragmentText());
  
  inherit =
    new QToolButton(*generalisationButton, TR("Generalisation"), QString::null,
		    this, SLOT(hit_inherit()), toolbar, "generalisation");
  inherit->setToggleButton(TRUE);
  QWhatsThis::add(inherit, inheritText());
  
  realize =
    new QToolButton(*realizeButton, TR("Realize"), QString::null,
		    this, SLOT(hit_realize()), toolbar, "realize");
  realize->setToggleButton(TRUE);
  QWhatsThis::add(realize, realizeText());
  
  dependency =
    new QToolButton(*dependencyButton, TR("Dependency"), QString::null,
		    this, SLOT(hit_dependency()), toolbar, "dependency");
  dependency->setToggleButton(TRUE);
  QWhatsThis::add(dependency, dependencyText());
  
  association =
    new QToolButton(*associationButton, TR("Association"), QString::null,
		    this, SLOT(hit_association()), toolbar, "association");
  association->setToggleButton(TRUE);
  QWhatsThis::add(association, associationText());
  
  directionalassociation =
    new QToolButton(*directionalAssociationExtensionButton, TR("Directional Association / Extension"), QString::null,
		    this, SLOT(hit_directionalassociation()), toolbar, "directional association");
  directionalassociation->setToggleButton(TRUE);
  QWhatsThis::add(directionalassociation, directionalassociationextensionText());
  
  aggregation =
    new QToolButton(*aggregationButton, TR("Aggregation"), QString::null,
		    this, SLOT(hit_aggregation()), toolbar, "aggregation");
  aggregation->setToggleButton(TRUE);
  QWhatsThis::add(aggregation, aggregationText());

  aggregationbyvalue =
    new QToolButton(*aggregationByValueButton, TR("Composition"), QString::null,
		    this, SLOT(hit_aggregationbyvalue()), toolbar, "composition");
  aggregationbyvalue->setToggleButton(TRUE);
  QWhatsThis::add(aggregationbyvalue, aggregationbyvalueText());
  
  directionalaggregation =
    new QToolButton(*directionalAggregationButton, TR("Directional Aggregation"), QString::null,
		    this, SLOT(hit_directionalaggregation()), toolbar, "directional aggregation");
  directionalaggregation->setToggleButton(TRUE);
  QWhatsThis::add(directionalaggregation, directionalaggregationText());
  
  directionalaggregationbyvalue =
    new QToolButton(*directionalAggregationByValueButton, TR("Directional Composition"), QString::null,
		    this, SLOT(hit_directionalaggregationbyvalue()), toolbar, "directional composition");
  directionalaggregationbyvalue->setToggleButton(TRUE);
  QWhatsThis::add(directionalaggregationbyvalue, directionalaggregationbyvalueText());
  
  note =
    new QToolButton(*noteButton, TR("Note"), QString::null,
		    this, SLOT(hit_note()), toolbar, "note");
  note->setToggleButton(TRUE);
  QWhatsThis::add(note, noteText());
  
  anchor =
    new QToolButton(*anchorButton, TR("Anchor / Class association"), QString::null,
		    this, SLOT(hit_anchor()), toolbar, "anchor");
  anchor->setToggleButton(TRUE);
  QWhatsThis::add(anchor, anchorText());
  
  text =
    new QToolButton(*textButton, TR("Text"), QString::null,
		    this, SLOT(hit_text()), toolbar, "text");
  text->setToggleButton(TRUE);
  QWhatsThis::add(text, textText());
  
  image =
    new QToolButton(*imageButton, TR("Image"), QString::null,
		    this, SLOT(hit_image()), toolbar, "image");
  image->setToggleButton(TRUE);
  QWhatsThis::add(image, imageText());
  
  toolbar->addSeparator();
  
  add_scale_cmd(toolbar);
  
  //
  
  view = new ClassDiagramView(this, canvas, (id != -1) ? id : b->get_ident());
  setFocusProxy(view);
  setCentralWidget(view);
  
  //qApp->setMainWidget(this);
  
  QWorkspace * w = UmlWindow::get_workspace();

  resize((w->width() * 4)/5, (w->height() * 4)/5);
  
  /*if (w->windowList().isEmpty())
    showMaximized();
  else*/
    show();
  
  view->preferred_size_zoom();
    
  //qApp->setMainWidget(0);
}

ClassDiagramWindow::~ClassDiagramWindow() {
  if (! no_save) {
    QString warning = "!";
    BooL is_new = FALSE;
    
    save("d", warning, is_new);
    warning.remove(0, 1);	// removes !
    view->hide();
    
    if (!warning.isEmpty())
      warn(warning);
  }
  
  browser_node->on_close();
}

DiagramView * ClassDiagramWindow::get_view() const {
  return view;
}

void ClassDiagramWindow::hit_button(UmlCode c, QToolButton * b) {
  view->abort_line_construction();
  
  select->setOn(FALSE);
  addClass->setOn(FALSE);
  addPackage->setOn(FALSE);
  addFragment->setOn(FALSE);
  inherit->setOn(FALSE);
  dependency->setOn(FALSE);
  association->setOn(FALSE);
  directionalassociation->setOn(FALSE);
  aggregation->setOn(FALSE);
  aggregationbyvalue->setOn(FALSE);
  directionalaggregation->setOn(FALSE);
  directionalaggregationbyvalue->setOn(FALSE);
  realize->setOn(FALSE);
  note->setOn(FALSE);
  anchor->setOn(FALSE);
  text->setOn(FALSE);
  image->setOn(FALSE);
  
  b->setOn(TRUE);
  current_button = c;
}

void ClassDiagramWindow::hit_class() {
  hit_button(UmlClass, addClass);
}

void ClassDiagramWindow::hit_package() {
  hit_button(UmlPackage, addPackage);
}

void ClassDiagramWindow::hit_fragment() {
  hit_button(UmlFragment, addFragment);
}

void ClassDiagramWindow::hit_inherit() {
  hit_button(UmlGeneralisation, inherit);
}

void ClassDiagramWindow::hit_dependency() {
  hit_button(UmlDependency, dependency);
}

void ClassDiagramWindow::hit_association() {
  hit_button(UmlAssociation, association);
}

void ClassDiagramWindow::hit_directionalassociation() {
  hit_button(UmlDirectionalAssociation, directionalassociation);
}

void ClassDiagramWindow::hit_aggregation() {
  hit_button(UmlAggregation, aggregation);
}

void ClassDiagramWindow::hit_aggregationbyvalue() {
  hit_button(UmlAggregationByValue, aggregationbyvalue);
}

void ClassDiagramWindow::hit_directionalaggregation() {
  hit_button(UmlDirectionalAggregation, directionalaggregation);
}

void ClassDiagramWindow::hit_directionalaggregationbyvalue() {
  hit_button(UmlDirectionalAggregationByValue, directionalaggregationbyvalue);
}

void ClassDiagramWindow::hit_realize() {
  hit_button(UmlRealize, realize);
}

void ClassDiagramWindow::hit_note() {
  hit_button(UmlNote, note);
}

void ClassDiagramWindow::hit_anchor() {
  hit_button(UmlAnchor, anchor);
}

void ClassDiagramWindow::hit_text() {
  hit_button(UmlText, text);
}

void ClassDiagramWindow::hit_image() {
  hit_button(UmlImage, image);
}

