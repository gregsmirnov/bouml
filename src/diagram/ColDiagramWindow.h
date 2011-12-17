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

#ifndef COLDIAGRAMWINDOW_H
#define COLDIAGRAMWINDOW_H

#include "DiagramWindow.h"

class QToolButton;
class QSpinBox;

class ColDiagramView;
class BrowserColDiagram;

class ColDiagramWindow : public DiagramWindow {
  Q_OBJECT
    
  protected:
    ColDiagramView * view;
    QToolButton * addClass;
    QToolButton * addClassInstance;
    QToolButton * addPackage;
    QToolButton * addFragment;
    QToolButton * addLink;
    QToolButton * addSelfLink;
    QToolButton * note;
    QToolButton * anchor;
    QToolButton * text;
    QToolButton * image;
  
  public:
    ColDiagramWindow(const QString & s, BrowserColDiagram *, int id = -1);
    ~ColDiagramWindow();

    virtual DiagramView * get_view() const;
    
    virtual void hit_button(UmlCode, QToolButton *);

  protected slots:
    void hit_class();
    void hit_classinstance();
    void hit_package();
    void hit_fragment();
    void hit_link();
    void hit_selflink();
    void hit_note();
    void hit_anchor();
    void hit_text();
    void hit_image();
};

#endif
