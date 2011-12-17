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

#ifndef NOTECANVAS_H
#define NOTECANVAS_H

#include "DiagramCanvas.h"

#define NOTE_CANVAS_MIN_SIZE 35
#define NOTE_MARGIN 10

class NoteCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  protected:
    UmlFont itsfont;
    UmlColor itscolor;
    UmlColor used_color;
    UmlColor fg_c;
    QString note;
  
    void save_internal(QTextStream & st) const;
    void read_internal(char * & st);

  public:
    NoteCanvas(UmlCanvas * canvas, int x, int y, int id);
    virtual ~NoteCanvas();
    
    virtual void delete_it();

    virtual void draw(QPainter & p);
    
    virtual UmlCode type() const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual aCorner on_resize_point(const QPoint &);
    virtual void resize(aCorner c, int dx, int dy, QPoint &);
    virtual void resize(const QSize & sz, bool w, bool h);
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(QList<DiagramItem> &);
    virtual void same_drawing_settings(QList<DiagramItem> &);
    void edit_drawing_settings();

    virtual void apply_shortcut(QString s);
  
    virtual void save(QTextStream  & st, bool ref, QString & warning) const;
    static NoteCanvas * read(char * &, UmlCanvas *, char *);
    
  protected slots:
    void modified();
};

#endif
