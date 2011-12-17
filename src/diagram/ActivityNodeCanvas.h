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

#ifndef ACTIVITYNODECANVAS_H
#define ACTIVITYNODECANVAS_H





#include <qobject.h>

#include "DiagramCanvas.h"
#include "Settings.h"

class ActivityNodeCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  protected:
    QPixmap * xpm;
    bool horiz;
    bool manual_size;

    ActivityNodeCanvas(UmlCanvas * canvas, int id);
    void set_xpm();
  
  public:
    ActivityNodeCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y);
    virtual ~ActivityNodeCanvas();
    
    virtual void delete_it();
    
    virtual void draw(QPainter & p);
    virtual void change_scale();
    
    virtual UmlCode type() const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual void remove(bool from_model);
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual aCorner on_resize_point(const QPoint & p);
    virtual void resize(aCorner c, int dx, int dy, QPoint &);
    virtual void resize(const QSize & sz, bool w, bool h);
    
    virtual void apply_shortcut(QString s);
  
    virtual void history_load(QBuffer &);
    virtual void history_save(QBuffer & b) const;
    virtual void history_hide();
    
    virtual void save(QTextStream  & st, bool ref, QString & warning) const;
    static ActivityNodeCanvas * read(char * &, UmlCanvas *, char *);
    virtual void post_loaded();
    
    bool force_inside();
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();
};

#endif
