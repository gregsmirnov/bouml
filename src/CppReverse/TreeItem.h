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

#ifndef TREEITEM_H
#define TREEITEM_H

#ifndef REVERSE

#include <qlistview.h>
#include <qstring.h>

#define TreeItem QListViewItem

#else

#include <qlist.h>
#include <qstring.h>

class TreeItem {
  private:
    TreeItem * its_parent;
    QList<TreeItem> its_children;
    QString its_name;
    
  public:
    TreeItem(TreeItem * parent, const char * n);
  
    QString text(int) { return its_name; };
    void setText(int, QString s) { its_name = s; };
    TreeItem * parent() { return its_parent; };
    
    TreeItem * firstChild() { return its_children.first(); };
    TreeItem * nextSibling() { return its_parent->its_children.next(); };
};

#endif

#endif
