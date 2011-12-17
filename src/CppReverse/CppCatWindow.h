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

#ifndef CPPCATWINDOW_H
#define CPPCATWINDOW_H

#ifdef REVERSE
#include "UmlCom.h"

#define CppCatWindow UmlCom

#else

#include <qmainwindow.h>

class QSplitter;
class BrowserView;
class BrowserNode;
class CommentView;
 
// singleton

class CppCatWindow : public QMainWindow {
  Q_OBJECT
    
  public:
    CppCatWindow();
  
    static void trace(QString);
    static void trace(QCString);
    static void clear_trace();
  
  protected:
    static CppCatWindow * the;
  
    QSplitter * spl;
    BrowserView * browser;
    CommentView * comment;
    BrowserNode * commented;    // the commented object    

private slots:
    void load();
    void save();
    void quit();
    void scan();
    void browser_search();
    void historic_back();
    void historic_forward();
    
    void motif_style();
    void motifplus_style();
    void windows_style();
    
    void about();
    void aboutQt();
};

#endif

#endif
