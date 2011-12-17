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

#ifndef SDDURATIONCANVAS_H
#define SDDURATIONCANVAS_H

#include "SdMsgSupport.h"
#include "SdDurationSupport.h"

#define DURATION_WIDTH 11

class SdObjCanvas;
class SdLifeLineCanvas;
class SdMsgBaseCanvas;
class ToolCom;

class SdDurationCanvas : public QObject, public SdMsgSupport, public SdDurationSupport {
  Q_OBJECT
    
  protected:
    SdDurationSupport * support;
    QList<SdDurationCanvas> durations;
    QList<SdMsgBaseCanvas> msgs;
    UmlColor itscolor;
    bool coregion;
  
    SdDurationCanvas(UmlCanvas * canvas, SdDurationSupport * sp,
		     int x, int y, int w, int h, int id, bool coreg);

    void save_sub(QTextStream &) const;
    void save_internal(QTextStream &) const;
    void cut_internal(int py);
    void update_self();
    void cut(const QPoint & p);
    void merge(QList<SdDurationCanvas> &);
    void collapse(SdDurationCanvas *);
    void toOverlapping(SdMsgBaseCanvas **, SdDurationCanvas * orig,
				    unsigned & index, unsigned sz);
    void postToOverlapping();

    static SdDurationCanvas * read_internal(char * & st, UmlCanvas *, int id, SdDurationSupport *);
    
  public:
    SdDurationCanvas(UmlCanvas * canvas, SdDurationSupport * sp,
		     int v, bool isdest);
    virtual ~SdDurationCanvas();
    
    virtual void delete_it();
    
    virtual void add(SdMsgBaseCanvas *);
    virtual void remove(SdMsgBaseCanvas *);
    virtual void add(SdDurationCanvas *);
    virtual void remove(SdDurationCanvas *);
    virtual void update_instance_dead();
    virtual void update_v_to_contain(SdDurationCanvas *, bool force);
    virtual int sub_x(int sub_w) const;
    virtual double min_y() const;
    virtual SdLifeLineCanvas * get_line() const;
    virtual bool isaDuration() const;
    virtual bool isOverlappingDuration() const;
    virtual double getZ() const;
    void go_up(SdMsgBaseCanvas *, bool isdest);
    void go_down(SdMsgBaseCanvas *);
    void update_hpos();
    virtual void update_v_to_contain(const QRect re);
    void toFlat();
    void toOverlapping();
    
    virtual void draw(QPainter & p);
    virtual void moveBy(double dx, double dy);
    virtual void prepare_for_move(bool on_resize);
    
    virtual UmlCode type() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual bool may_connect(UmlCode l) const;
    virtual aCorner on_resize_point(const QPoint &);
    virtual void resize(int w, int h);
    virtual void resize(aCorner c, int dx, int dy, QPoint &);
    virtual void resize(const QSize & sz, bool w, bool h);
    virtual void change_scale();
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual bool connexion(UmlCode, const QPoint &, const QPoint &);
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual LineDirection allowed_direction(UmlCode);
    virtual void select_associated();
    virtual bool copyable() const;
    virtual void history_save(QBuffer & b) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(QList<DiagramItem> &);
    virtual void same_drawing_settings(QList<DiagramItem> &);
  
    virtual void apply_shortcut(QString s);
    void edit_drawing_settings();
  
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static SdDurationCanvas * read(char * & st, UmlCanvas * canvas, char *);
    static SdDurationCanvas * read(char * & st, UmlCanvas * canvas, bool ref);
    
    unsigned count_msg(int api_format) const;
    void send(ToolCom * com, int id) const;
    
    static void propag_visible(QList<SdDurationCanvas> &, bool y);
    static void propag_dz(QList<SdDurationCanvas> &, double dz);
    
  private slots:
    void modified();
};

#endif
