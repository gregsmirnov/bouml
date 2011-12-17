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

#ifndef ICONCANVAS_H
#define ICONCANVAS_H

#include "DiagramCanvas.h"

#define ICON_CANVAS_MIN_SIZE 16

class IconCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  public:
    IconCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y, int id);
    IconCanvas(UmlCanvas * canvas);
    virtual ~IconCanvas();

    virtual void draw(QPainter & p);
    virtual void change_scale();
    
    virtual UmlCode type() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual bool represents(BrowserNode *);
    virtual void apply_shortcut(QString s);
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static IconCanvas * read(char * &, UmlCanvas *, char *);
    
  private slots:
    void deleted();	// the browser_node is deleted
};

#endif
