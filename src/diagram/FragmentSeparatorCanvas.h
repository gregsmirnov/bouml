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

#ifndef FRAGMENTSEPARATORCANVAS_H
#define FRAGMENTSEPARATORCANVAS_H

#include "DiagramCanvas.h"

class FragmentCanvas;

class FragmentSeparatorCanvas : public DiagramCanvas {
  protected:
    FragmentCanvas * fragment;
    double vpos;
  
  public:
    FragmentSeparatorCanvas(UmlCanvas * canvas, FragmentCanvas * f, double vp = 0.5);
    virtual ~FragmentSeparatorCanvas();
    
    virtual void delete_it();

    void compute_position();
    void update();
    
    virtual void drawShape(QPainter & p);
    virtual void moveBy(double dx, double dy);
    
    virtual UmlCode type() const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual LineDirection allowed_direction(UmlCode);
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual void open();
    virtual void menu(const QPoint&);
    virtual void change_scale();
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode &, const DiagramItem *) const;
    
    virtual void save(QTextStream &, bool ref, QString & warning) const;
    static FragmentSeparatorCanvas * read(char * &, UmlCanvas *,
					  char * k, FragmentCanvas * f);
    
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
};

#endif
