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

#include "BrowserNode.h"
#include "BrowserView.h"
#include "Pixmap.h"
#include "myio.h"

#define TREE_COL 0
#define REVISION_COL 1
#define MODIFIEDBY_COL 2

#ifdef __APPLE__
#define CTRL "Apple"
#else
#define CTRL "Ctrl"
#endif

QDict<BrowserNode> BrowserNode::Youngs;
    
BrowserNode::BrowserNode(BrowserView * parent, QString fn)
    : QListViewItem(parent), filename(fn), view(parent), state(Unknown) {
  parent->add_node(this);
}

BrowserNode::BrowserNode(BrowserNode * parent, QString fn, BrowserView * v)
    : QListViewItem(parent), filename(fn), view(v), state(Unknown) {
  v->add_node(this);
     
  // move it at end
  QListViewItem * child = parent->firstChild();
  
  while (child->nextSibling())
    child = child->nextSibling();
  
  if (child != this)
    moveItem(child);
  
  parent->setOpen(TRUE);
}

void BrowserNode::set_state(State st)
{
  state = st;
  
  if (state == Young)
    Youngs.replace(filename, this);
}

int BrowserNode::get_rev() const {
  return atoi(text(REVISION_COL));
}

// solve state and return TRUE if Young
bool BrowserNode::solve() { 
  if (state != Unknown)
    return (state == Young);
  
  // note : parent can't be 0
  BrowserNode * p = (BrowserNode *) parent();
  
  if ((p->state == Unknown) ? p->solve() : (p->state == Young)) {
    state = Young;
    return TRUE;
  }
  else {
    state = Deleted;
    return FALSE;
  }
}

const QPixmap * BrowserNode::pixmap(int col) const {
  if (col != 0)
    return 0;

  switch (state) {
  case UpToDate:
    return UpToDatePackageIcon;
  case Young:
    return YoungPackageIcon;
  case Old:
    return OldPackageIcon;
  case Deleted:
    return DeletedPackageIcon;
  default:
    return PackageIcon;
  }
}

// load package definition
bool BrowserNode::load(QDir & dir) {
  BooL ro = FALSE;
  char * buff = read_file(dir, filename, ro);
  
  if (buff == 0)
    return FALSE;
  
  char * p = buff;
  
  read_token(p);	// 'format'
  read_token(p);	// format
  
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
	setText(MODIFIEDBY_COL, modifier_name + " [" + QString::number(n) + "]");
	set_user_name(n, k);
      }
      
      k = read_token(p);
    }
  }
  else {
    setText(REVISION_COL, "0");
  }
  
  QStringList sub_packages_id;
  
  while (k != 0) {
    if (! strcmp(k, "package_ref"))
      sub_packages_id.append(read_token(p));
    else if (! strcmp(k, "simplerelation")) {
      // may contain package_ref
      do 
	k = read_token(p);
      while ((k != 0) && strcmp(k, "end"));
    }
    else if (! strcmp(k, "class")) {
      QString fn = read_token(p) + QString(".bodies");
      
      classes.append(fn);
      ro |= is_readonly(dir, fn);
    }
    else if (! strcmp(k, "activitydiagram") ||
	     ! strcmp(k, "classdiagram") ||
	     ! strcmp(k, "deploymentdiagram") ||
	     ! strcmp(k, "collaborationdiagram") ||
	     ! strcmp(k, "componentdiagram") ||
	     ! strcmp(k, "deploymentdiagram") ||
	     ! strcmp(k, "objectdiagram") ||
	     ! strcmp(k, "sequencediagram") ||
	     ! strcmp(k, "statediagram") ||
	     ! strcmp(k, "usecasediagram")) {
      QString fn = read_token(p) + QString(".diagram");
      
      diagrams.append(fn);
      ro |= is_readonly(dir, fn);
    }
	     
    k = read_token(p);
  }
  
  if (ro)
    view->set_readonly();
  
  delete [] buff;
  
  QStringList::Iterator it;
  
  for (it = sub_packages_id.begin(); it != sub_packages_id.end(); ++it) {
     BrowserNode * bn = new BrowserNode(this, *it, view);
     
     if (! bn->load(dir))
       return FALSE;
  }
  
  return TRUE;
}

// synchronize all in todir, current packages are 'nodes'
void BrowserNode::synchronize(QDir & todir, QDict<BrowserNode> & nodes)
{
  static QDict<void> useful(9973);	// all useful files
  static bool made_useful = TRUE;	// set at the first call
  
  // compare nodes with young packages
  
  QDictIterator<BrowserNode> ity(Youngs);
  
  for (; ity.current(); ++ity) {
    BrowserNode * from = ity.current();
    BrowserNode * curr_bn = nodes.find(ity.currentKey());
    
    if ((curr_bn == 0) || (curr_bn->state == Old)) {
      // don't exist or must be updated
      if (made_useful)
	useful.insert(ity.currentKey(), (void *) 1);

      QDir & fromdir = from->view->get_dir();
    
      // copy package file
      copy(fromdir, todir, from->filename);
      
      // copy diagrams files
      QStringList::Iterator its;
      QStringList & diags = from->diagrams;
      
      if (made_useful) {
	for (its = diags.begin(); its != diags.end(); ++its) {
	  copy_if_needed(fromdir, todir, *its);
	  useful.insert(*its, (void *) 1);
	}
      }
      else
	for (its = diags.begin(); its != diags.end(); ++its)
	  copy_if_needed(fromdir, todir, *its);

      // copy class body files
      QStringList & cls = from->classes;
      
      if (made_useful) {
	for (its = cls.begin(); its != cls.end(); ++its) {
	  copy_if_needed(fromdir, todir, *its);
	  useful.insert(*its, (void *) 1);
	}
      }
      else
	for (its = cls.begin(); its != cls.end(); ++its)
	  copy_if_needed(fromdir, todir, *its);
      
      if (from->parent() == 0) {
	// project, special files
	copy_if_needed(fromdir, todir, "cpp_includes");
	copy_if_needed(fromdir, todir, "idl_includes");
	copy_if_needed(fromdir, todir, "java_imports");
	copy_if_needed(fromdir, todir, "generation_settings");
	copy_if_needed(fromdir, todir, "import");
	copy_if_needed(fromdir, todir, "include");
	copy_if_needed(fromdir, todir, "stereotypes");
	copy_if_needed(fromdir, todir, "tools");
      }
    }
    else if (made_useful)
      // young, memorize packages, diagrams and classes body file
      from->memo(useful); 
  }
  
  if (made_useful) {
    // memorize up to date packages, diagrams and classes body file
    QDictIterator<BrowserNode> itn(nodes);
    
    for (; itn.current(); ++itn)
      if (itn.current()->state == UpToDate)
	itn.current()->memo(useful); 
    
    made_useful = FALSE;
  }
  
  // remove files associated to deleted elements
  purge(todir, useful);
}

// this is useful, memorize it and its diagrams and classes body file
void BrowserNode::memo(QDict<void> & useful) {
  useful.insert(filename, (void *) 1);
  
  QStringList::Iterator its;
  
  for (its = diagrams.begin(); its != diagrams.end(); ++its)
    useful.insert(*its, (void *) 1);
  
  for (its = classes.begin(); its != classes.end(); ++its)
    useful.insert(*its, (void *) 1);
}

