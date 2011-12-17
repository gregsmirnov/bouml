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

#ifndef UMLCANVAS
#define UMLCANVAS





#include <qcanvas.h>
#include <qfont.h>
#include <qintdict.h> 

#include "UmlEnum.h"
#include "Labeled.h"

class QCanvasLine;
class QPainter;
class BrowserNode;
class BrowserDiagram;
class DiagramView;
class DiagramItem;

class UmlCanvas : public QCanvas {
  Q_OBJECT
    
  protected:
    QCanvasItemList selected;
    BrowserDiagram * br_diagram;
    DiagramView * view;
    bool do_scale;
    bool do_paste;
    bool show_shadow;
    bool draw_all_relations;
    bool saved_draw_all_relations;
    double zoom_value;
    int width100percent;
    int height100percent;
    QFont the_fonts[UmlFontNumber];
    IdDict<DiagramItem> all_items;	// to re-use the same id for the DiagramItems
    QCanvasLine * hlimit;
    QCanvasLine * vlimit;
    
    void update_limits();

  public:
    UmlCanvas(CanvasFormat f, BrowserDiagram * br_diag);
    ~UmlCanvas();
    
    QCanvasItem * collision(const QPoint &) const;
    QCanvasItem * collision(const QPoint &, int except) const;
    const QCanvasItemList & selection() const { return selected; };
    void select(QCanvasItem * i);
    void unselect(QCanvasItem * i);
    void unselect_all();

    void del(QCanvasItem * i);
    
    void set_zoom(double zm);
    void zoom_end() { do_scale = FALSE; };
    bool do_zoom() const { return do_scale; };
    double zoom() const { return zoom_value; };
    
    void resize(CanvasFormat);
    
    int shadow() const;
    bool must_draw_all_relations() const { return draw_all_relations; }
    void dont_draw_all_relations();
    void freeze_draw_all_relations() {
      saved_draw_all_relations = draw_all_relations;
      draw_all_relations = FALSE;
    }
    void unfreeze_draw_all_relations() { draw_all_relations = saved_draw_all_relations; }
    static void update_global_settings();
  
    QFont get_font(UmlFont f) const { return the_fonts[f]; };
    
    BrowserDiagram * browser_diagram() const { return br_diagram; };
    void set_view(DiagramView * v);
    DiagramView * get_view() const { return view; };
    
    IdDict<DiagramItem> & get_all_items() { return all_items; }
    void read_old_diagram(bool y) { all_items.read_old_diagram(y); }
    void set_paste(bool y) { do_paste = y; all_items.read_old_diagram(y); }
    bool paste() { return do_paste; }
    void clear_old_ids() { all_items.clear(TRUE); }
    
    void show_limits(bool y);
    
    bool already_drawn(BrowserNode * bn);
    
  public slots:
    virtual void update();
};

#endif
