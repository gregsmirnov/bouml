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





#include <stdlib.h>

#include <qapplication.h>
#include <qpopupmenu.h>
#include <qcursor.h>

#include "BrowserNode.h"
#include "BrowserView.h"
#include "Pixmap.h"
#include "myio.h"

#define TREE_COL 0
#define OWNER_COL 1
#define OWNERNAME_COL 2
#define REVISION_COL 3
#define MODIFIEDBY_COL 4

#ifdef __APPLE__
#define CTRL "Apple"
#else
#define CTRL "Ctrl"
#endif

BrowserNode::BrowserNode(BrowserView * parent, QString fn)
    : QListViewItem(parent), filename(fn) {
}

BrowserNode::BrowserNode(BrowserNode * parent, QString fn)
    : QListViewItem(parent), filename(fn) {
  // move it at end
  QListViewItem * child = parent->firstChild();
  
  while (child->nextSibling())
    child = child->nextSibling();
  
  if (child != this)
    moveItem(child);
}

const QPixmap * BrowserNode::pixmap(int col) const {
  if (col != 0)
    return 0;
  
  if (owner == -1)
    return (ro) ? RoFreePackageIcon : FreePackageIcon;
  else if (owner < 2)
    return (ro) ? RoProtectedPackageIcon : ProtectedPackageIcon;
  else if (owner == user_id())
    return (ro) ? RoUserPackageIcon: UserPackageIcon;
  else
    return (ro) ? RoOtherPackageIcon : OtherPackageIcon;
}

bool BrowserNode::load(QDir & dir) {
  char * buff = read_file(dir, filename, ro);
  
  if (buff == 0)
    return FALSE;
  
  char * p = buff;
  
  read_token(p);	// format
  format = atoi(read_token(p));
  
  setText(TREE_COL, read_string(p));
  
  char * k = read_token(p);
  
  if (!strcmp(k, "revision")) {
    setText(REVISION_COL, read_token(p));
    k = read_token(p);
    
    if (!strcmp(k, "modified_by")) {
      int n = atoi(read_token(p));
      
      k = read_string(p);
      
      if ((n >= 2) || (n <= 127)) {
	modifier_name = k;
	modifier_id = n;
	setText(MODIFIEDBY_COL, modifier_name + " [id " + QString::number(n) + "]");
	set_user_name(n, k);
      }
      
      k = read_token(p);
    }
    
    if (!strcmp(k, "owner")) {
      k = read_token(p);
      owner = atoi(k);
      
      if ((owner < 2) || (owner > 127))
	setText(OWNER_COL, "<protected>");
      else {
	setText(OWNER_COL, k);
	
	char * s = read_string(p);
	
	set_user_name(owner, s);
	setText(OWNERNAME_COL, s);
      }
      
      k = read_token(p);
    }
    else
      owner = -1;
  }
  else {
    owner = -1;
    setText(REVISION_COL, "0");
  }
  
  offset = p - buff - strlen(k);
  
  QStringList sub_packages_id;
  
  while (k != 0) {
    if (! strcmp(k, "package_ref"))
      sub_packages_id.append(read_token(p));
    else if (! strcmp(k, "simplerelation")) {
      // may contain a package_ref, bypass
      do 
	k = read_token(p);
      while ((k != 0) && strcmp(k, "end"));
    }
    else if (! strcmp(k, "oid")) {
      (void) read_token(p);
      ro = TRUE;
    }
    k = read_token(p);
  }
  
  delete [] buff;
  
  QStringList::Iterator it;
  
  for (it = sub_packages_id.begin(); it != sub_packages_id.end(); ++it)
    if (! (new BrowserNode(this, *it))->load(dir))
      return FALSE;
  
  return TRUE;
}

void BrowserNode::key_event(QKeyEvent * e) {
  // control or alt is pressed with p, u or a
  QApplication::setOverrideCursor(Qt::waitCursor);
  if (e->state() == ::Qt::ControlButton) {
    switch (e->key()) {
    case ::Qt::Key_A:
      assign(user_id());
      break;
    case ::Qt::Key_P:
      assign(0);
      break;
    default: // ::Qt::Key_U
      assign(-1);
      break;
    }
  }
  else {
    switch (e->key()) {
    case ::Qt::Key_A:
      assign_all(user_id());
      break;
    case ::Qt::Key_P:
      assign_all(0);
      break;
    default: // ::Qt::Key_U
      assign_all(-1);
      break;
    }
  }
  QApplication::restoreOverrideCursor();
}

void BrowserNode::menu() {
  QString who = user_name(user_id());
  
  if (who.isEmpty())
    who = "<" + QString::number(user_id()) + ">";
  
  QPopupMenu m(0);
  
  m.insertItem("Package " + text(0), -1);

  if (! ro) {
    m.insertSeparator();
    if (owner == user_id()) {
      m.insertItem("Protect this package ("CTRL"+p)", 3);
      m.insertItem("Unassign this package ("CTRL"+u)", 2);
    }
    else if (owner == -1) {
      m.insertItem("Protect this package ("CTRL"+p)", 3);
      m.insertItem("Assign this package to " + who + " ("CTRL"+a)", 1);
    }
    else if (owner < 2) {
      m.insertItem("Assign this package to " + who + " ("CTRL"+a)", 1);
      m.insertItem("Unassign this package (Ctrl+u)", 2);
    }
    else {
      m.insertItem("Protect this package ("CTRL"+p)", 3);
      m.insertItem("Unassign this package ("CTRL"+u)", 2);
      m.insertItem("Assign this package to " + who + " ("CTRL"+a)", 1);
    }
  }
  
  if (firstChild() != 0) {
    m.insertSeparator();
    m.insertItem("Protect " + who + "'s packages recursively from this one", 10);
    m.insertItem("Protect unassigned packages recursively from this one", 11);
    m.insertItem("Protect all packages recursively from this one (Alt+p)", 12);
    m.insertItem("Assign to " + who + " unassigned packages recursively from this one", 13);
    m.insertItem("Assign to " + who + " all packages recursively from this one (Alt+a)", 14);
    m.insertItem("Unassign " + who + "'s packages recursively from this one", 15);
    m.insertItem("Unassign all packages recursively from this one (Alt+u)", 16);
  }
  else if (ro)
    return;
  
  int choice = m.exec(QCursor::pos());
  
  QApplication::setOverrideCursor(Qt::waitCursor);
  
  switch (choice) {
  case 1:
    assign(user_id());
    break;
  case 2:
    assign(-1);
    break;
  case 3:
    assign(0);
    break;
  case 10:
    assign_mine(0);
    break;
  case 11:
    assign_unassigned(0);
    break;
  case 12:
    assign_all(0);
    break;
  case 13:
    assign_unassigned(user_id());
    break;
  case 14:
    assign_all(user_id());
    break;
  case 15:
    assign_mine(-1);
    break;
  case 16:
    assign_all(-1);
  default:
    break;
  }
  
  QApplication::restoreOverrideCursor();
}

void BrowserNode::assign(int w) {
  if (!ro && (owner != w)) {
    QDir & dir = BrowserView::get_dir();
    BooL dummy;
    
    char * p = read_file(dir, filename, dummy);
    
    if (p != 0) {
      QString path = dir.absFilePath(filename);
      FILE * fp = fopen(path, "wb");
      
      if (fp != 0) {
	fprintf(fp, "format %d\n\"%s\"\n  revision %s\n",
		format, (const char *) text(TREE_COL), (const char *) text(REVISION_COL));
	
	if (!modifier_name.isEmpty())
	  fprintf(fp, "  modified_by %d \"%s\"\n", modifier_id, (const char *) modifier_name);
	
	if (w != -1) {
	  fprintf(fp, "  owner %d", w);
	  
	  if (w != 0) {
	    QString s = user_name(w);
	    
	    setText(OWNER_COL, QString::number(w));
	    setText(OWNERNAME_COL, s);
	    
	    if (! s.isEmpty())
	      fprintf(fp, " \"%s\"\n", (const char *) s);
	    else
	      fputs(" \"\"\n", fp);
	  }
	  else {
	    fputc('\n', fp);
	    
	    setText(OWNER_COL, "<protected>");
	    setText(OWNERNAME_COL, "");
	  }
	}
	else {
	  setText(OWNER_COL, "");
	  setText(OWNERNAME_COL, "");
	}
	
	fputs("  ", fp);
	
	int new_offset = (int) ftell(fp);
	
	fputs(p + offset, fp);
	fclose(fp);
	
	offset = new_offset;
	owner = w;
	repaint();
      }
      
      delete [] p;
    }
  }
}

void BrowserNode::assign_mine(int w) {
  if (owner == user_id())
    assign(w);
  
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling())
    ((BrowserNode *) child)->assign_mine(w);
}

void BrowserNode::assign_unassigned(int w) {
  if (owner == -1)
    assign(w);
  
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling())
    ((BrowserNode *) child)->assign_unassigned(w);
}

void BrowserNode::assign_all(int w) {
  assign(w);
  
  QListViewItem * child;
    
  for (child = firstChild(); child != 0; child = child->nextSibling())
    ((BrowserNode *) child)->assign_all(w);
}

//

void BrowserNodeList::search(BrowserNode * bn, const QString & s, bool cs)
{
  QListViewItem * child;
    
  for (child = bn->firstChild(); child != 0; child = child->nextSibling()) {
    if (child->text(TREE_COL).find(s, 0, cs) != -1)
      append((BrowserNode *) child);
    
    search((BrowserNode *) child, s, cs);
  }
}

int BrowserNodeList::compareItems(QCollection::Item item1, QCollection::Item item2)
{
  return ((BrowserNode *) item1)->text(TREE_COL)
    .compare(((BrowserNode *) item2)->text(TREE_COL));
}
