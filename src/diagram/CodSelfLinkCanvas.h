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

#ifndef CODSELFLINKCANVAS_H
#define CODSELFLINKCANVAS_H

#include "DiagramCanvas.h"
#include "MultipleDependency.h"
#include "CodMsgSupport.h"

class CodObjCanvas;
class LabelCanvas;
class ColMsg;

#define SELF_LINK_RADIUS 16

class CodSelfLinkCanvas : public QObject, public DiagramCanvas, public CodMsgSupport,
  			  public MultipleDependency<BasicData> {
  Q_OBJECT
    
  protected:
    int delta_x;
    int delta_y;
    double angle;
    CodObjCanvas * obj;
  
  public:
    CodSelfLinkCanvas(UmlCanvas * canvas, CodObjCanvas * o,
		      const QPoint & p, int id);
    virtual ~CodSelfLinkCanvas();
    
    void update_pos();
    void compute_pos(QPoint p);
    void update_label_pos();
    virtual void update_msgs();
    virtual void get_from_to(CodObjCanvas *& from, CodObjCanvas *& to,
			     bool forward);
    virtual bool copyable() const;
    
    virtual void delete_it();
    virtual void remove_it(ColMsg * msg);

    virtual void draw(QPainter & p);
    virtual void moveBy(double dx, double dy);
    
    virtual UmlCode type() const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;    
    virtual bool represents(BrowserNode *);
    virtual void save(QTextStream & st, bool ref, QString & warning) const;
    static CodSelfLinkCanvas * read(char * & st, UmlCanvas * canvas, char * & k);
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
  protected slots:
    void modified();
};

#endif
