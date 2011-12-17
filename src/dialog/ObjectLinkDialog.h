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

#ifndef OBJECTLINKDIALOG_H
#define OBJECTLINKDIALOG_H

#include <qlist.h>
#include <qdialog.h>

class MyTable;
class RelationData;
class BrowserClassInstance;

class ObjectLinkDialog : public QDialog {
  Q_OBJECT
    
  public:
    ObjectLinkDialog(BrowserClassInstance * a, BrowserClassInstance * b,
		     QList<RelationData> & l, RelationData * current,
		     int nfirstdir);
    virtual ~ObjectLinkDialog();
  
    RelationData * rel() const { return choozen; }
    bool rev() const { return reverse; }
    
  protected:
    void init(RelationData * current);
    
    QList<RelationData> & rels;
    int nforward;
    int ninputrels;
    BrowserClassInstance * clia;
    BrowserClassInstance * clib;
    QString ra;
    QString rb;
    MyTable * table;
    RelationData * choozen;
    bool reverse;
    
    static QSize previous_size;
    
  protected slots:
    virtual void polish();
    virtual void accept();
    virtual void unselect();
    virtual void create();
};

#endif
