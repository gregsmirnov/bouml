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

#ifndef HUBCANVAS_H
#define HUBCANVAS_H

#include "DiagramCanvas.h"

#define HUB_SIZE 6

class HubCanvas : public DiagramCanvas {
  public:
    HubCanvas(UmlCanvas * canvas, int x, int y, int id);
    virtual ~HubCanvas();
    
    virtual void draw(QPainter & p);
    
    virtual UmlCode type() const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual void change_scale();
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual void save(QTextStream & st, bool ref, QString & warning) const;
    static HubCanvas * read(char * & st, UmlCanvas * canvas, char * k);
};

#endif
