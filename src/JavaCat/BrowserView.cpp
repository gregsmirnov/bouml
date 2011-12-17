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

#include "BrowserView.h"
#include "BrowserNode.h"

BrowserView * BrowserView::the;

BrowserView::BrowserView(QWidget * parent) : QListView(parent) {
  the = this;
  
  setSorting(-1);		// manual sorting
  addColumn("browser          ");
  setTreeStepSize(18);
  
  connect(this, SIGNAL(selectionChanged(QListViewItem*)),
	  this, SLOT(selected(QListViewItem*)));
  
  connect(this, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
	  this, SLOT(rightPressed(QListViewItem *)));

}

void BrowserView::select(QListViewItem * b)
{
  the->ensureItemVisible(b);
  the->setSelected(b, TRUE);
}

void BrowserView::selected(QListViewItem * b)
{
  ((BrowserNode *) b)->activated();
}

void BrowserView::rightPressed(QListViewItem * item) {
  if (item != 0)
    ((BrowserNode *) item)->menu();
}

void BrowserView::refer(const QString & href) {
  BrowserNode * bn = (BrowserNode *) selectedItem();
  
  if (bn != 0)
    bn->refer(href);
}
