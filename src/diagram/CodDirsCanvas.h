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

#ifndef CODDIRSCANVAS_H
#define CODDIRSCANVAS_H

#include "DiagramCanvas.h"
#include "MultipleDependency.h"
#include "CodMsgSupport.h"

class CodLinkCanvas;
class QTextStream;

#define COL_MSG_Z 1000
#define OLD_COL_MSG_Z 0.2

#define COL_DIRS_SIZE 50

#define isa_col_msg_dirs(x) ((x)->rtti() == RTTI_COL_MSG)

class CodDirsCanvas : public QObject, public DiagramCanvas, public CodMsgSupport,
  		      public MultipleDependency<BasicData> {
  Q_OBJECT
    
  protected:
    double angle;
    
    LabelCanvas * backward_label;
    CodLinkCanvas * link;	// to get start & end
    
  public:
    CodDirsCanvas(UmlCanvas * canvas, CodLinkCanvas * l, int id);
    virtual ~CodDirsCanvas();
  
    virtual void delete_it();
    virtual void remove_it(ColMsg * msg);
    
    virtual void get_from_to(CodObjCanvas *& from, CodObjCanvas *& to,
			     bool forward);
    
    virtual UmlCode type() const;
    virtual int rtti() const;
    virtual void open();
    virtual void menu(const QPoint &);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode &, const DiagramItem *) const;
    virtual bool copyable() const;
  
    void set_link(CodLinkCanvas * l) { link = l; };
    void update_pos(const QPoint & link_start, const QPoint & link_end);
    void update_label_pos(LabelCanvas *, bool forward);
    virtual void update_msgs();
    bool edit_drawing_settings();
    virtual void draw(QPainter & p);
    virtual void setVisible(bool yes);
    
    virtual bool represents(BrowserNode *);
    
    virtual void save(QTextStream & st, bool, QString & warning) const;
    static CodDirsCanvas * read(char * & st, UmlCanvas * canvas, char * & k);
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
  private slots:
    void modified();	// messages must be updated
    //void deleted();	// the relation is deleted
};

#endif
