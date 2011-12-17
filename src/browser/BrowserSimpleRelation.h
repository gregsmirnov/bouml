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

#ifndef BROWSER_SIMPLE_RELATION_H
#define BROWSER_SIMPLE_RELATION_H

template <class K> class QPtrDict;
class QPixmap;

class SimpleRelationData;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserSimpleRelation : public BrowserNode, public Labeled<BrowserSimpleRelation> {
  protected:
    static IdDict<BrowserSimpleRelation> all;
  
    SimpleRelationData * def;
    
  protected:
    void exec_menu_choice(int rank);
    
  public:
    BrowserSimpleRelation(BrowserNode * p, SimpleRelationData * d, int id = 0);
    BrowserSimpleRelation(const BrowserSimpleRelation * model, BrowserNode * p);
    BrowserSimpleRelation(int id);
    virtual ~BrowserSimpleRelation();
  
    virtual bool undelete(bool rec, QString & warning, QString & renamed);
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
        
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual bool in_edition() const;
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void update_stereotype(bool = FALSE);
    
    virtual void write_id(ToolCom * com);
    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserSimpleRelation * read_ref(char * & st);
    static BrowserSimpleRelation * read(char * &, char *, BrowserNode *);
    static BrowserSimpleRelation * temporary(SimpleRelationData * d);
    static BrowserNode * get_it(const char * k, int id);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual void referenced_by(QList<BrowserNode> & l, bool ondelete);
    static void compute_referenced_by(QList<BrowserNode> &, BrowserNode *);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);

    static QString drag_key(BrowserNode * p);
    virtual QString drag_key() const;
    virtual QString drag_postfix() const;
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    
    static void get_relating(BrowserNode *, QPtrDict<BrowserNode> & d,
			     BrowserNodeList & newones, bool inh,
			     bool dep, bool sametype, UmlCode k);
};

#endif

