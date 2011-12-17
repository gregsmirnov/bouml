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
#include <qspinbox.h>

#include "UmlWindow.h"
#include "SeqDiagramWindow.h"
#include "SeqDiagramView.h"
#include "BrowserSeqDiagram.h"
#include "UmlPixmap.h"
#include "myio.h"
#include "translate.h"

QString addmodeledclassinstanceText() { return TR("Click this button to add a modeled <i>class instance</i> in the diagram. <br><br>"
						  "You can also drop the class instance from the <b>browser</b>."); }
QString addclassinstanceText() { return TR("Click this button to add a <i>class instance</i> in the diagram. <br><br>"
					   "You can also drop the class from the <b>browser</b>."); }
extern QString addfragmentText();
extern QString noteText();
extern QString anchorText();
QString syncmsgText() { return TR("Click this button to send a <i>synchronous message</i>"); }
QString asyncmsgText() { return TR("Click this button to send an <i>asynchronous message</i>"); }
QString syncselfmsgText() { return TR("Click this button to send a <i>reflexive synchronous message</i>"); }
QString asyncselfmsgText() { return TR("Click this button to send a <i>reflexive asynchronous message</i>"); }
QString lostsyncmsgText() { return TR("Click this button to send a <i>synchronous lost message</i>"); }
QString lostasyncmsgText() { return TR("Click this button to send a <i>asynchronous lost message</i>"); }
QString foundsyncmsgText() { return TR("Click this button to add a <i>synchronous found message</i>"); }
QString foundasyncmsgText() { return TR("Click this button to add a <i>asynchronous found message</i>"); }
QString returnmsgText() { return TR("Click this button to add an <i>explicitly return</i>"); }
QString selfreturnmsgText() { return TR("Click this button to add an <i>explicit reflexive return</i>"); }
QString textText() { return TR("Click this button to add a <i>text</i>"); }
QString addcontinuationText() { return TR("Click this button to add a <i>continuation</i>"); }
extern QString imageText();

// id is an old ident in case of an import
SeqDiagramWindow::SeqDiagramWindow(const QString & s, BrowserSeqDiagram * b, int id)
    : DiagramWindow(b, s), view(0) {
  QToolBar * toolbar = new QToolBar(this, "sequence diagram operations");
  addToolBar(toolbar, TR("Toolbar"), Top, TRUE);
  
  add_edit_button(toolbar);
  
  select =
    new QToolButton(*selectButton, TR("Select"), QString::null,
		    this, SLOT(hit_select()), toolbar, 
		    "select");
  select->setToggleButton(TRUE);
  select->setOn(TRUE);
  current_button = UmlSelect;
  
  addFragment
    = new QToolButton(*fragmentButton, TR("Add Fragment"), QString::null,
		      this, SLOT(hit_fragment()), toolbar,
		      "add fragment");
  addFragment->setToggleButton(TRUE);
  QWhatsThis::add(addFragment, addfragmentText());
  
  addClassInstance
    = new QToolButton(*classinstanceButton, TR("Add modeled Class instance"), QString::null,
		      this, SLOT(hit_classinstance()), toolbar,
		      "add modeled class instance");
  addClassInstance->setToggleButton(TRUE);
  QWhatsThis::add(addClassInstance, addmodeledclassinstanceText());
  
  addClass
    = new QToolButton(*classButton, TR("Add Class instance"), QString::null,
		      this, SLOT(hit_class()), toolbar,
		      "add class");
  addClass->setToggleButton(TRUE);
  QWhatsThis::add(addClass, addclassinstanceText());
  
  addContinuation
    = new QToolButton(*continuationButton, TR("Add Continuation"), QString::null,
		      this, SLOT(hit_continuation()), toolbar,
		      "add continuation");
  addContinuation->setToggleButton(TRUE);
  QWhatsThis::add(addContinuation, addcontinuationText());
  
  syncMsg =
    new QToolButton(*syncmsgButton, TR("Synchronous message"), QString::null,
		    this, SLOT(hit_syncmsg()), toolbar,
		    "synchronous message");
  syncMsg->setToggleButton(TRUE);
  QWhatsThis::add(syncMsg, syncmsgText());
  
  asyncMsg =
    new QToolButton(*directionalAssociationButton, TR("Asynchronous message"), QString::null,
		    this, SLOT(hit_asyncmsg()), toolbar,
		    "asynchronous message");
  asyncMsg->setToggleButton(TRUE);
  QWhatsThis::add(asyncMsg, asyncmsgText());
  
  syncSelfMsg =
    new QToolButton(*syncselfmsgButton, TR("Synchronous Reflexive message"), QString::null,
		    this, SLOT(hit_syncselfmsg()), toolbar,
		    "synchronous reflexive message");
  syncSelfMsg->setToggleButton(TRUE);
  QWhatsThis::add(syncSelfMsg, syncselfmsgText());
  
  asyncSelfMsg =
    new QToolButton(*asyncselfmsgButton, TR("Asynchronous Reflexive message"), QString::null,
		    this, SLOT(hit_asyncselfmsg()), toolbar,
		    "asynchronous reflexive message");
  asyncSelfMsg->setToggleButton(TRUE);
  QWhatsThis::add(asyncSelfMsg, asyncselfmsgText());
  
  foundsyncMsg =
    new QToolButton(*foundsyncmsgButton, TR("Synchronous found message"), QString::null,
		    this, SLOT(hit_foundsyncmsg()), toolbar,
		    "synchronous found message");
  foundsyncMsg->setToggleButton(TRUE);
  QWhatsThis::add(foundsyncMsg, foundsyncmsgText());
  
  foundasyncMsg =
    new QToolButton(*foundasyncmsgButton, TR("Asynchronous found message"), QString::null,
		    this, SLOT(hit_foundasyncmsg()), toolbar,
		    "asynchronous found message");
  foundasyncMsg->setToggleButton(TRUE);
  QWhatsThis::add(foundasyncMsg, foundasyncmsgText());
  
  lostsyncMsg =
    new QToolButton(*lostsyncmsgButton, TR("Synchronous lost message"), QString::null,
		    this, SLOT(hit_lostsyncmsg()), toolbar,
		    "synchronous lost message");
  lostsyncMsg->setToggleButton(TRUE);
  QWhatsThis::add(lostsyncMsg, lostsyncmsgText());
  
  lostasyncMsg =
    new QToolButton(*lostasyncmsgButton, TR("Asynchronous lost message"), QString::null,
		    this, SLOT(hit_lostasyncmsg()), toolbar,
		    "asynchronous lost message");
  lostasyncMsg->setToggleButton(TRUE);
  QWhatsThis::add(lostasyncMsg, lostasyncmsgText());
  
  returnMsg =
    new QToolButton(*returnmsgButton, TR("Explicit return"), QString::null,
		    this, SLOT(hit_returnmsg()), toolbar,
		    "explicit return");
  returnMsg->setToggleButton(TRUE);
  QWhatsThis::add(returnMsg, returnmsgText());
  
  selfreturnMsg =
    new QToolButton(*selfreturnmsgButton, TR("Explicit reflexive return"), QString::null,
		    this, SLOT(hit_selfreturnmsg()), toolbar,
		    "explicit reflexive return");
  selfreturnMsg->setToggleButton(TRUE);
  QWhatsThis::add( selfreturnMsg,  selfreturnmsgText());
  
  note =
    new QToolButton(*noteButton, TR("Note"), QString::null,
		    this, SLOT(hit_note()), toolbar, 
		    "note");
  note->setToggleButton(TRUE);
  QWhatsThis::add(note, noteText());
  
  anchor =
    new QToolButton(*anchorButton, TR("Anchor"), QString::null,
		    this, SLOT(hit_anchor()), toolbar,
		    "anchor");
  anchor->setToggleButton(TRUE);
  QWhatsThis::add(anchor, anchorText());
  
  text =
    new QToolButton(*textButton, TR("Text"), QString::null,
		    this, SLOT(hit_text()), toolbar,
		    "text");
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
  
  view = new SeqDiagramView(this, canvas, (id != -1) ? id : b->get_ident());
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

SeqDiagramWindow::~SeqDiagramWindow() {
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

DiagramView * SeqDiagramWindow::get_view() const {
  return view;
}

void SeqDiagramWindow::hit_button(UmlCode c, QToolButton * b) {
  view->abort_line_construction();
  
  select->setOn(FALSE);
  addFragment->setOn(FALSE);
  addClass->setOn(FALSE);
  addClassInstance->setOn(FALSE);
  addContinuation->setOn(FALSE);
  syncMsg->setOn(FALSE);
  asyncMsg->setOn(FALSE);
  foundsyncMsg->setOn(FALSE);
  foundasyncMsg->setOn(FALSE);
  lostsyncMsg->setOn(FALSE);
  lostasyncMsg->setOn(FALSE);
  syncSelfMsg->setOn(FALSE);
  asyncSelfMsg->setOn(FALSE);
  selfreturnMsg->setOn(FALSE);
  returnMsg->setOn(FALSE);
  note->setOn(FALSE);
  anchor->setOn(FALSE);
  text->setOn(FALSE);
  image->setOn(FALSE);
  
  b->setOn(TRUE);
  current_button = c;
}

void SeqDiagramWindow::hit_fragment() {
  hit_button(UmlFragment, addFragment);
}

void SeqDiagramWindow::hit_classinstance() {
  hit_button(UmlClassInstance, addClassInstance);
}

void SeqDiagramWindow::hit_class() {
  hit_button(UmlClass, addClass);
}

void SeqDiagramWindow::hit_continuation() {
  hit_button(UmlContinuation, addContinuation);
}

void SeqDiagramWindow::hit_syncmsg() {
  hit_button(UmlSyncMsg, syncMsg);
}

void SeqDiagramWindow::hit_asyncmsg() {
  hit_button(UmlAsyncMsg, asyncMsg);
}

void SeqDiagramWindow::hit_lostsyncmsg() {
  hit_button(UmlLostSyncMsg, lostsyncMsg);
}

void SeqDiagramWindow::hit_lostasyncmsg() {
  hit_button(UmlLostAsyncMsg, lostasyncMsg);
}

void SeqDiagramWindow::hit_foundsyncmsg() {
  hit_button(UmlFoundSyncMsg, foundsyncMsg);
}

void SeqDiagramWindow::hit_foundasyncmsg() {
  hit_button(UmlFoundAsyncMsg, foundasyncMsg);
}

void SeqDiagramWindow::hit_syncselfmsg() {
  hit_button(UmlSyncSelfMsg, syncSelfMsg);
}

void SeqDiagramWindow::hit_asyncselfmsg() {
  hit_button(UmlAsyncSelfMsg, asyncSelfMsg);
}

void SeqDiagramWindow::hit_returnmsg() {
  hit_button(UmlReturnMsg, returnMsg);
}

void SeqDiagramWindow::hit_selfreturnmsg() {
  hit_button(UmlSelfReturnMsg, selfreturnMsg);
}

void SeqDiagramWindow::hit_note() {
  hit_button(UmlNote, note);
}

void SeqDiagramWindow::hit_anchor() {
  hit_button(UmlAnchor, anchor);
}

void SeqDiagramWindow::hit_text() {
  hit_button(UmlText, text);
}

void SeqDiagramWindow::hit_image() {
  hit_button(UmlImage, image);
}

