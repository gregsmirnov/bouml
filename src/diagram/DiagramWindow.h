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

#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <qcanvas.h>
#include <qmainwindow.h>
#include <qtextstream.h>

#include "UmlEnum.h"

class QPrinter;
class QToolButton;
class QSpinBox;
class QComboBox;
class QToolBar;

class BrowserDiagram;
class UmlCanvas;
class DiagramView;

#define SCALE_MIN 30
#define SCALE_MAX 200

class DiagramWindow : public QMainWindow {
  Q_OBJECT
    
  protected:
    bool no_save;
    UmlCanvas * canvas;
    BrowserDiagram * browser_node;
    UmlCode current_button;
  
    QToolButton * select;
    QSpinBox * sb_zoom;
    QToolButton * optwinsize;
    QComboBox * viewmode;
    QToolButton * edit;






  public:
    DiagramWindow(BrowserDiagram * br, const QString & s);
    ~DiagramWindow();
  
    virtual DiagramView * get_view() const = 0;
    
    void raise();
    
    virtual void hit_button(UmlCode, QToolButton *) = 0;
    void selectOn();
    UmlCode & buttonOn() { return current_button; };
    
    void add_edit_button(QToolBar *);
    void add_scale_cmd(QToolBar *);
    void change_zoom(int);
    
    BrowserDiagram * browser_diagram() const { return browser_node; };
    void package_modified() const;
    bool frozen() const;

    void save(const char * ext, QString & warning, BooL & is_new) const;
    void duplicate(int dest_id, const char * ext) const;
    QString copy_selected() const;

    void dont_save() { no_save = TRUE; };
    
    void save_session(QTextStream & st);
    void read_session(char * & st);
        
  public slots:
    void new_scale(int);
    void fit_scale();
    void optimal_window_size();
    void session_window_size();
    virtual void call_menu();
    
  protected slots:
    void hit_select();
};

#endif
