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

#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H

#include <qcanvas.h>
#include <qtextstream.h>

#include "UmlEnum.h"
#include "UmlCanvas.h"

#ifndef QT_NO_PRINTER
class QPrinter;
#endif
class QKeyEvent;
class QPopupMenu;
class QBuffer;

class DiagramItem;
class DiagramWindow;

#define EDIT_DRAWING_SETTING_CMD 0
#define RELOAD_CMD 6

class DiagramView : public QCanvasView {
  Q_OBJECT
    
  protected:
    int id;
    int pressedButton;
    QPoint mousePressPos;	// mouse position when it is pressed or moved
    QCanvasRectangle * selectArea;
    DiagramItem * start;	// last added line part start
    QCanvasLine * line;
    DiagramItem * arrowBeginning;// broken line start
    QCanvasItemList temp;
    QSize preferred_size;
    double preferred_zoom;
    bool draw_line;
    aCorner do_resize;
    bool history_protected;
    bool history_frozen;
    bool first_move;
    bool on_arrow_decenter;
    BooL decenter_start;
    BooL decenter_horiz;
    QValueList<QPoint> previousResizeCorrection;
    QList<QByteArray> history;
    unsigned history_index;




    static QCString clipboard;
    static UmlCode copied_from;
    
  public:
    DiagramView(QWidget * parent, UmlCanvas * canvas, int i);
  
    void select_all();
    void unselect_all() { the_canvas()->unselect_all(); };
    const QCanvasItemList & selection() { return the_canvas()->selection(); };
    void select(QCanvasItem * i) { the_canvas()->select(i); };
    void unselect(QCanvasItem * i) { the_canvas()->unselect(i); };
    void abort_line_construction();
    void relation_to_simplerelation(UmlCode k);
    void set_zoom(double);
    void fit_scale();
    void preferred_size_zoom();
    bool has_preferred_size_zoom() { return preferred_zoom != 0; }
    void multiple_selection_menu(bool in_model, bool out_model, bool alignable,
				 int n_resize, QList<DiagramItem> & l_drawing_settings);
    bool is_present(BrowserNode * bn);
    virtual void add_related_elements(DiagramItem *, QString what,
				      bool inh, bool assoc);
    
    virtual UmlCanvas * the_canvas() const;

    virtual void menu(const QPoint&) = 0;
    
    void read();
    void paste();
    virtual void read(char *, char * k) = 0;
    virtual void save(QTextStream & st, QString & warning, bool copy) const = 0;
    void load(const char *);
    
    void protect_history(bool y) { history_protected = y; }
    void freeze_history(bool y) { history_frozen = y; }
    void history_save(bool on_undo = FALSE);
    void history_load();
    void update_history();
    void undo();
    void redo();
    bool available_undo();
    bool available_redo();
      
    void save_session(QTextStream & st);
    void read_session(char * & st);
    
    static void init();
    
#ifndef QT_NO_PRINTER
    void print(QPrinter & p, int div);
#endif
    
    void copy_in_clipboard(bool optimal, bool temporary);
    bool save_pict(const char * f, bool optimal, bool temporary);
    bool save_in(const char * f, bool optimal, bool temporary);
    bool svg_save_in(const char * f, bool optimal, bool temporary);
    
    void renumber(int ident);
    
    void do_optimal_window_size();
    
  private:
    DiagramWindow * window() const {
      return (DiagramWindow *) parent();
    };

  protected:
    virtual void contentsMouseDoubleClickEvent(QMouseEvent *);
    virtual void contentsMousePressEvent(QMouseEvent *);
    virtual void contentsMouseReleaseEvent(QMouseEvent *);
    virtual void contentsMouseMoveEvent(QMouseEvent *);
    virtual void contentsWheelEvent(QWheelEvent *);
    void moveSelected(int dx, int dy, bool first);
    void resizeSelected(int dx, int dy);
    void add_point(QMouseEvent * e);
    bool multiple_selection_for_menu(BooL & in_model, BooL & out_model,
				     BooL & alignable, int & n_resize,
				     QList<DiagramItem> & l_drawing_settings,
				     const QCanvasItemList & selected);
    
    void set_format(int);
    void init_format_menu(QPopupMenu &, QPopupMenu &, int) const;
    int default_menu(QPopupMenu & m, int f);
    void needed_width_height(int & maxx, int & maxy) const;
    
    void save_picture(bool optimal, bool svg);
    
    void alignLeft();
    void alignRight();
    void alignTop();
    void alignBottom();
    void alignCenter();
    void alignHorizontaly();
    void alignVerticaly();
    void same_size(bool w, bool h);
    
    void delete_them(bool in_model);
    
  protected slots:
    virtual void keyPressEvent(QKeyEvent * e);
  private slots:
    virtual void keyReleaseEvent(QKeyEvent *);
    void optimal_window_size();
    void restore_window_size();
    
    void update() { canvas()->update(); };
};

#endif
