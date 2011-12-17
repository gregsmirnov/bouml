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

#ifndef COMPONENTDIAGRAMWINDOW_H
#define COMPONENTDISGRAMWINDOW_H

#include "DiagramWindow.h"

class QToolButton;
class QSpinBox;

class ComponentDiagramView;
class BrowserComponentDiagram;

class ComponentDiagramWindow : public DiagramWindow {
  Q_OBJECT
    
  protected:
    ComponentDiagramView * view;
    QToolButton * addComponent;
    QToolButton * addPackage;
    QToolButton * addFragment;
    QToolButton * inherit;
    QToolButton * dependency;
    QToolButton * required;
    QToolButton * provided;
    QToolButton * note;
    QToolButton * anchor;
    QToolButton * text;
    QToolButton * image;
  
  public:
    ComponentDiagramWindow(const QString & s, BrowserComponentDiagram * b, int id = -1);
    ~ComponentDiagramWindow();

    virtual DiagramView * get_view() const;
    
    virtual void hit_button(UmlCode, QToolButton *);
    
  protected slots:
    void hit_component();
    void hit_package();
    void hit_fragment();
    void hit_inherit();
    void hit_dependency();
    void hit_required();
    void hit_provided();
    void hit_note();
    void hit_anchor();
    void hit_text();
    void hit_image();
};

#endif
