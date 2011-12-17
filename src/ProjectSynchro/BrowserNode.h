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
#include <qdict.h>

class QDir;
class QKeyEvent;
class BrowserView;

enum State {
  UpToDate, Young, Old, Deleted, Unknown
};

class BrowserNode : public QListViewItem {
  public:
    BrowserNode(BrowserView * parent, QString fn);
    BrowserNode(BrowserNode * parent, QString fn, BrowserView * v);
    virtual ~BrowserNode() {};	// to avoid compiler warning
    
    virtual const QPixmap * pixmap(int) const;
    
    bool load(QDir & dir);
    QString file_name() const { return filename; }
    
    void set_state(State);
    State get_state() const { return state; }
    
    int get_rev() const;
    
    bool solve();	// solve state and return TRUE if Young
    static void synchronize(QDir & todir, QDict<BrowserNode> & nodes);
    
  private:
    
    void memo(QDict<void> & useful);

    QString filename;
    QString modifier_name;
    int modifier_id;
    BrowserView * view;
    State state;
    QStringList diagrams;
    QStringList classes;
    
    static QDict<BrowserNode> Youngs;  
};

#endif
