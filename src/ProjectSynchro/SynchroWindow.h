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

#ifndef SYNCHROWINDOW_H
#define SYNCHROWINDOW_H

#include <qmainwindow.h>
#include <qlist.h>

class QHBox;
class BrowserView;
class BrowserNode;
 
// singleton

class SynchroWindow : public QMainWindow {
  Q_OBJECT
    
  public:
    SynchroWindow();
    ~SynchroWindow();
    
    void load(int argc, char ** argv);
    
    static const QList<BrowserView> & get_browsers() {
      return the->browsers;
    }
    static void abort();
  
  protected:
    static SynchroWindow * the;
  
    QString project_name;
    QList<BrowserView> browsers;
    QHBox * hbox;
    
    void load(QString path);

  private slots:
    void load();
    void quit();
    void synchronize();
    
    void motif_style();
    void motifplus_style();
    void windows_style();
    
    void about();
    void aboutQt();
};

#endif
