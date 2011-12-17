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

#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <qmainwindow.h>
#include <qstringlist.h>

class BrowserView;
class BrowserNode;
class QDir;
 
// singleton

class ControlWindow : public QMainWindow {
  Q_OBJECT
    
  public:
    ControlWindow(QDir & homeDir);
    ~ControlWindow();
  
  protected:
    static ControlWindow * the;
  
    BrowserView * browser;
    QStringList historic;
    
    void load(QString path);
    void show_identity();

  private slots:
    void load();
    void quit();
    void browser_search();
    void historicActivated(int id);
    void change_user();

    
    void motif_style();
    void motifplus_style();
    void windows_style();
    
    void about();
    void aboutQt();
};

#endif
