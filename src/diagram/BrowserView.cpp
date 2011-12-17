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
#include <qpopupmenu.h> 
#include <qdragobject.h>
#include <qheader.h>
#include <qcursor.h>

#include "BrowserView.h"
#include "UmlWindow.h"
#include "BrowserPackage.h"
#include "UmlDrag.h"
#include "BrowserSearchDialog.h"
#include "Shortcut.h"
#include "myio.h"
#include "mu.h"
#include "translate.h"

BrowserPackage * BrowserView::project;
BrowserPackage * BrowserView::imported_project;
QDir BrowserView::dir;
QDir BrowserView::import_dir;
BrowserView * BrowserView::the;

BrowserView::BrowserView(QWidget * parent) : QListView(parent) {
  the = this;
  
  project = 0;			// no project yet
  mousePressed = FALSE;
  
  setSorting(-1);		// manual sorting
  addColumn(TR("browser          "));
  setTreeStepSize(18);
  //setSelectionMode(Extended);	// multi selection
  header()->setClickEnabled(TRUE);
  setAcceptDrops(TRUE);
  viewport()->setAcceptDrops(TRUE);
  setDragAutoScroll(TRUE);
  
  connect(this, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
	  this, SLOT(rightPressed(QListViewItem *)));
  
  connect(this, SIGNAL(doubleClicked(QListViewItem *)),
  	  this, SLOT(doubleClick(QListViewItem *)));
  
  connect(header(), SIGNAL(clicked(int)), this, SLOT(menu()));
  
  connect(this, SIGNAL(selectionChanged(QListViewItem*)),
	  this, SLOT(selected(QListViewItem*)));
}
  
void BrowserView::remove_temporary_files()
{
  if (project) {
    QString filter;
    
    filter.sprintf("*_%d.*", user_id());
    
    const QFileInfoList * l = dir.entryInfoList(filter);
    
    if (l) {
      QListIterator<QFileInfo> it(*l);
      QFileInfo *fi;
      
      while ((fi = it.current()) != 0) {
	if (fi->extension(FALSE).lower() != "prj")
	  QFile::remove(fi->absFilePath());
	++it;
      }
    }
  }
}

BrowserView::~BrowserView() {
  remove_temporary_files();
  set_user_id(-1);
}

void BrowserView::clear() {
  remove_temporary_files();
  set_user_id(-1);
  
  QListView::clear();
  project = 0;
}

void BrowserView::set_project(const QDir & di) {
  dir = di;
  project = new BrowserPackage(dir.dirName(), the, PROJECT_ID);
  setRootIsDecorated(TRUE/*FALSE*/);
}

void BrowserView::set_imported_project(const QDir & di,
				       BrowserPackage * p) {
  import_dir = di;
  imported_project = p;
}

bool BrowserView::save_as(const QDir & new_dir) {
  // copy USER temporary files
  QString filter;
  
  filter.sprintf("*_%d.?", user_id());
  
  const QFileInfoList * l = dir.entryInfoList(filter);
  
  if (l) {
    QListIterator<QFileInfo> it(*l);
    QFileInfo *fi;
    
    while ((fi = it.current()) != 0) {
      if (!copy_file(fi, new_dir))
	return FALSE;
      ++it;
    }
  }
  
  // copy non package non temporary files
  l = dir.entryInfoList("*.??*");
  
  if (l) {
    QListIterator<QFileInfo> it(*l);
    QFileInfo *fi;
    
    while ((fi = it.current()) != 0) {
      if ((fi->extension(FALSE) != "prj") && 
	  (fi->extension(FALSE) != "lock") &&
	  !copy_file(fi, new_dir))
	return FALSE;
      ++it;
    }
  }
  
  // delete old temporary files and rename the project
  remove_temporary_files();
  set_user_id(-1);
  get_project()->set_name(new_dir.dirName());
  dir = new_dir;
  
  // create new lock
  user_id();
  
  return TRUE;
}

void BrowserView::select(QListViewItem * i) {
  the->ensureItemVisible(i);
  the->setSelected(i, TRUE);
}

void BrowserView::force_visible(QListViewItem * i) {
  the->ensureItemVisible(i);
}

BrowserNode * BrowserView::selected_item()
{
  return (BrowserNode *) the->selectedItem();
}

void BrowserView::rightPressed(QListViewItem * item) {
  if (item &&
      ((!((BrowserNode *) item)->deletedp()) ||
       (!((BrowserNode *) item->parent())->deletedp())) &&
      !BrowserNode::popupMenuActive()) {	// Qt bug
    BrowserNode::setPopupMenuActive(TRUE);
    ((BrowserNode *) item)->menu();
    BrowserNode::setPopupMenuActive(FALSE);
  }
}

void BrowserView::doubleClick(QListViewItem * item) {
  UmlWindow::abort_line_construction();
  
  if (item && !((BrowserNode *) item)->deletedp())
    ((BrowserNode *) item)->open(FALSE);
}

void BrowserView::contentsDragMoveEvent(QDragMoveEvent * e) {
  if (!BrowserNode::edition_active()) {
    QListViewItem * item = itemAt(contentsToViewport(e->pos()));
    
    if (item && !((BrowserNode *) item)->deletedp())
      ((BrowserNode *) item)->DragMoveEvent(e);
  }
}

void BrowserView::contentsDropEvent(QDropEvent * e) {
  if (!BrowserNode::edition_active()) {
    QListViewItem * item = itemAt(contentsToViewport(e->pos()));
    
    if (item && !((BrowserNode *) item)->deletedp())
      ((BrowserNode *) item)->DropEvent(e);
  }
}

void BrowserView::contentsMousePressEvent(QMouseEvent * e) {
  UmlWindow::abort_line_construction();
  
  QPoint p(contentsToViewport(e->pos()));
  QListViewItem * i = itemAt(p);
  
  if (i != 0) {
    // to force update of comment else nothing done
    // when the click is made on the already selected item
    // (but the comment is one of an other element selected
    // in a diagram)
    selected(i);
    
    if (e->button() == ::Qt::LeftButton) {
      if (!((BrowserNode *) i)->deletedp() && (i != project)) {
	if (e->state() & ::Qt::ControlButton)
	  ((BrowserNode *) i)->toggle_mark();
	else if (e->pos().x() > header()->cellPos(header()->mapToActual(0)) +
		 treeStepSize() * (i->depth() + (rootIsDecorated() ? 1 : 0)) + itemMargin()/* ||
											      p.x() < header()->cellPos(header()->mapToActual(0))*/) {
	  // if the user clicked into the root decoration of the item, don't try to start a drag!
	  presspos = e->pos();
	  mousePressed = TRUE;
	}
      }
    }
  }
  
  QListView::contentsMousePressEvent(e);
}

void BrowserView::contentsMouseMoveEvent(QMouseEvent * e) {
  if (mousePressed &&
      ((presspos - e->pos()).manhattanLength() > QApplication::startDragDistance())) {
    mousePressed = FALSE;
    
    QListViewItem * item = itemAt(contentsToViewport(presspos));
    
    if (item) {
      UmlDrag * di = new UmlDrag((BrowserNode *) item, this);
      
      di->setPixmap(*(item->pixmap(0)), QPoint(8,8));
      di->dragMove();
    }
  }
}

void BrowserView::contentsMouseReleaseEvent(QMouseEvent *) {
  mousePressed = FALSE;
}

void BrowserView::keyPressEvent(QKeyEvent * e) {
  UmlWindow::abort_line_construction();
  
  QString s = Shortcut::shortcut(e->key(), e->state());
  
  if (!s.isEmpty()) {
    e->accept();

    if (s == "Save")
      UmlWindow::save_it();
    else if (s == "Save as")
      UmlWindow::saveas_it();
    else if (s == "Close")
      UmlWindow::do_close();
    else if (s == "Quit")
      UmlWindow::do_quit();
    else if (s == "Browser search")
      UmlWindow::browser_search_it();
    else if (s == "Open project")
      UmlWindow::load_it();
    else {
      BrowserNode * bn = (BrowserNode *) selectedItem();
      
      if (bn != 0) {
	if (s == "Menu")
	  rightPressed(bn);
	else if (s == "Delete") {
	  QApplication::setOverrideCursor(::Qt::waitCursor);
	  bn->apply_shortcut("Delete");
	  QApplication::restoreOverrideCursor();
	}
	else if ((s != "Move left") && (s != "Move right") &&
		 (s != "Move up") && (s != "Move down"))
	  bn->apply_shortcut(s);
	else
          QListView::keyPressEvent(e);
      }
    }
  }
  else {
    // no shortcut
    QListView::keyPressEvent(e);
  }
}

void BrowserView::menu() {
  if (project != 0) {
    if (BrowserSearchDialog::get() == 0)
      (new BrowserSearchDialog())->show();
    else
      BrowserSearchDialog::get()->raise();
  }
}

void BrowserView::selected(QListViewItem * b)
{
  UmlWindow::set_commented((BrowserNode *) b);
}
