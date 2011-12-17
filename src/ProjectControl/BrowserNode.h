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

#ifndef BROWSERNODE_H
#define BROWSERNODE_H

#include <qlistview.h>

class QDir;
class QKeyEvent;
class BrowserView;

class BrowserNode : public QListViewItem {
  public:
    BrowserNode(BrowserView * parent, QString fn);
    BrowserNode(BrowserNode * parent, QString fn);
    virtual ~BrowserNode() {};	// to avoid compiler warning
    
    virtual const QPixmap * pixmap(int) const;
    
    void menu();
    QString get_path() const { return path; }
    bool load(QDir & dir);
    void key_event(QKeyEvent *);
    
  private:
    void assign(int);
    void assign_mine(int);
    void assign_unassigned(int);
    void assign_all(int);

  private:
    QString filename;
    QString path;
    QString modifier_name;
    int modifier_id;
    int owner;
    int format;
    int offset;
    BooL ro;
};

// a sortable list of BrowserNode

#include <qlist.h>

class BrowserNodeList : public QList<BrowserNode> {
  public:
    void search(BrowserNode * bn, const QString & s, bool cs);
    virtual int compareItems(QCollection::Item item1, QCollection::Item item2);
};

#endif
