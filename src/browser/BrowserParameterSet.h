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

#ifndef BROWSER_PARAMETERSET_H
#define BROWSER_PARAMETERSET_H

class QPixmap;
class ParameterSetData;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserParameterSet : public BrowserNode, public Labeled<BrowserParameterSet> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserParameterSet> all;
    static QStringList its_default_stereotypes;
  
    ParameterSetData * def;
    
  protected:
    BrowserParameterSet(int id);
  
    void exec_menu_choice(int rank);
    
  public:
    BrowserParameterSet(QString s, BrowserNode * p, ParameterSetData * d, int id = 0);
    BrowserParameterSet(const BrowserParameterSet * model, BrowserNode * p);
    virtual ~BrowserParameterSet();
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual bool allow_empty() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    
    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserParameterSet * read_ref(char * &);
    static BrowserParameterSet * read(char * &, char *, BrowserNode *);
    static BrowserNode * get_it(const char * k, int id);

    static BrowserParameterSet * new_one(const char *, BrowserNode * p);
    
    virtual void referenced_by(QList<BrowserNode> & l, bool ondelete);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
        
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    QString drag_key() const;
    QString drag_postfix() const;
    static QString drag_key(BrowserNode * p);
    
    static void init();
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(QTextStream &);
};

#endif

