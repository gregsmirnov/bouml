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

#ifndef ASSOCCONTAINCANVAS_H
#define ASSOCCONTAINCANVAS_H

#include "ArrowCanvas.h"

class AssocContainCanvas : public ArrowCanvas {
  public:
    AssocContainCanvas(UmlCanvas * canvas, DiagramItem * b,
		     DiagramItem * e, int id, float d_start, float d_end);
    virtual ~AssocContainCanvas();
    
    virtual ArrowPointCanvas * brk(const QPoint &);
    
    virtual void open();
    virtual void menu(const QPoint&);
    virtual void remove(bool from_model);
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    
    virtual void save(QTextStream & st, bool ref, QString & warning) const;
    static AssocContainCanvas * read(char * & st, UmlCanvas * canvas, char * k);
};

#endif
