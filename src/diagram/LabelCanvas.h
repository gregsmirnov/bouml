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

#ifndef LABELCANVAS_H
#define LABELCANVAS_H

#include <qfont.h>

#include "qcanvas.h"

#include "DiagramItem.h"
#include "UmlCanvas.h"

#define LABEL_Z 3000
#define OLD_LABEL_Z 1e100

#define isa_label(x) ((x)->rtti() == RTTI_LABEL)

class LabelCanvas : public QCanvasText, public DiagramItem {
  public:
    static const char * Triangle;
    static const char * Zigzag;
  
  protected:
    bool multi_lines_centered;
    int center_x_scale100;
    int center_y_scale100;
    
  public:
    LabelCanvas(const QString & n, UmlCanvas * canvas, int x, int y,
		bool bold = FALSE, bool italic = FALSE,
		bool underlined = FALSE, bool mlcentered = TRUE);
    virtual ~LabelCanvas();
    
    virtual void delete_it();

    const QString get_name() const { return text(); };
    virtual QRect rect() const;
    int width() const { return boundingRect().width(); };
    int height() const { return boundingRect().height(); };
    void set_name(const QString &);
    
    void set_strikeout(bool yes);
    void set_underlined(bool yes);
    
    void move_outside(QRect r, double angle);
    
    virtual void draw(QPainter & p);
    
    virtual UmlCode type() const;
    virtual int rtti() const;
    void moveBy(double dx, double dy);
    virtual QPoint center() const;
    void recenter();
    void set_center100();
    virtual bool contains(int, int) const;
    virtual void change_scale();
    virtual void open();
    void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual double get_z() const;
    virtual void set_z(double z);	// only called by upper() & lower()
    virtual UmlCanvas * the_canvas() const;
    virtual bool isSelected() const;
    virtual void save(QTextStream  & st, bool ref, QString & warning) const;
    static LabelCanvas * read(char * & st, UmlCanvas * canvas, char *);
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual void check_stereotypeproperties();
};

#endif
