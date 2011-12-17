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

#ifndef BROWSER_EXTRAMEMBER_H
#define BROWSER_EXTRAMEMBER_H

class QPixmap;
class ExtraMemberData;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserExtraMember : public BrowserNode, public Labeled<BrowserExtraMember> {
  protected:
    static IdDict<BrowserExtraMember> all;
  
    ExtraMemberData * def;
    
  protected:
    BrowserExtraMember(int id);
  
    void exec_menu_choice(int rank);
    
  public:
    BrowserExtraMember(QString s, BrowserNode * p, ExtraMemberData * d, int id = 0);
    BrowserExtraMember(const BrowserExtraMember * model, BrowserNode * p);
    virtual ~BrowserExtraMember();
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual const char * help_topic() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual void set_name(const char * s);
    virtual bool same_name(const QString & s, UmlCode type) const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void member_cpp_def(const QString & prefix, const QString &, 
				QString & s, bool templ) const;
    virtual bool allow_empty() const;
    
    virtual void save(QTextStream &, bool ref, QString & warning);
    static BrowserExtraMember * read(char * &, char *, BrowserNode *);
    static BrowserNode * get_it(const char * k, int id);

    static BrowserExtraMember * new_one(QString s, BrowserNode * p);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
};

#endif

