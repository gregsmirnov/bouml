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

#ifndef TEXTCANVAS_H
#define TEXTCANVAS_H

#include "DiagramCanvas.h"

class ToolCom;

#define TEXT_CANVAS_MIN_SIZE 15

class TextCanvas : public DiagramCanvas {
  protected:
    UmlFont itsfont;
    UmlColor fg_c;
    UmlColor bg_c;
    QString text;
  
  public:
    TextCanvas(UmlCanvas * canvas, int x, int y, int id);
    virtual ~TextCanvas();

    virtual void draw(QPainter & p);
    
    virtual UmlCode type() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual bool move_with(UmlCode) const;
    virtual aCorner on_resize_point(const QPoint &);
    virtual void resize(aCorner c, int dx, int dy, QPoint &);
    virtual void resize(const QSize & sz, bool w, bool h);
    virtual void apply_shortcut(QString s);
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static TextCanvas * read(char * &, UmlCanvas *, char *);
    
    static void send(ToolCom * com, QCanvasItemList & all);
};

#endif
