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

#ifndef BROWSERVIEW_H
#define BROWSERVIEW_H

#include <qlistview.h> 
#include <qdir.h>

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QKeyEvent;

class BrowserNode;
class BrowserPackage;

// in fact it is a singleton => static members (except slots !)

class BrowserView : public QListView  {
  Q_OBJECT
    
  private:
    static BrowserView * the;
    static BrowserPackage * project;
    static QDir dir;
    static BrowserPackage * imported_project;
    static QDir import_dir;
    
    QPoint presspos;
    bool mousePressed;
  
  public:
    BrowserView(QWidget * parent = 0);
    virtual ~BrowserView();
  
    virtual void clear();
  
    void set_project(const QDir & d);
    bool save_as(const QDir & new_dir);

    static BrowserPackage * get_project() {
      return project;
    }
    static QDir get_dir() {
      return dir;
    }
    static void set_imported_project(const QDir &, BrowserPackage *);
    static BrowserPackage * get_imported_project() {
      return imported_project;
    }
    static QDir get_import_dir() {
      return import_dir;
    }
    static void select(QListViewItem *);
    static BrowserNode * selected_item();
    static void force_visible(QListViewItem *);
    static void remove_temporary_files();
      
  protected:
    void keyPressEvent(QKeyEvent * e);

  protected slots:
    void selected(QListViewItem *);
    void rightPressed(QListViewItem *);
    void doubleClick(QListViewItem *);
    void menu();

    void contentsDragMoveEvent(QDragMoveEvent * e);
    void contentsDropEvent(QDropEvent * e);
    void contentsMouseMoveEvent(QMouseEvent * e);
    void contentsMousePressEvent(QMouseEvent * e);
    void contentsMouseReleaseEvent(QMouseEvent * e);
};

#endif
