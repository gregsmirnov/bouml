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

#ifndef CLASSINSTANCEDATA_H
#define CLASSINSTANCEDATA_H

#include <qptrdict.h>

#include "SimpleData.h"

class BrowserClass;
class RelationData;
class BrowserAttribute;
class BrowserClassInstance;

struct SlotAttr {
  BrowserAttribute * att;
  QString value;	// unicode
  
  SlotAttr(BrowserAttribute * a, QString v) 
    : att(a), value(v) {}
  
  SlotAttr() : att(0) {}	// for qvaluelist
};

struct SlotRel {
  // warning : don't save BrowserRelation because side B 
  // may be 'really' deleted 
  bool is_a;
  RelationData * rel;
  BrowserClassInstance * value;
  
  SlotRel(bool a, RelationData * r, BrowserClassInstance * v) 
    : is_a(a), rel(r), value(v) {}
  
  SlotRel() : is_a(FALSE), rel(0), value(0) {}	// for qvaluelist
};

class ClassInstanceData : public SimpleData {
  Q_OBJECT
    
  friend class ClassInstanceDialog;
  
  private:
    BrowserClass * cl;	// type of instance
    QValueList<SlotAttr> attributes;
    QValueList<SlotRel> relations;
    QList<BasicData> connect_list;	// ClassData and ClassInstanceData
    
  protected:
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    void replace_internal(BrowserClassInstance * other,
			  RelationData * current, RelationData * future,
			  bool current_isa, bool future_isa);
    void remove_clone(BrowserClassInstance * other,
		      RelationData * rd, bool isa);
    bool change_rel(ToolCom * com, const char * args, bool add);
    
  public:
    ClassInstanceData(BrowserClass *);
    ClassInstanceData(ClassInstanceData * model);
    virtual ~ClassInstanceData();
    
    void edit();
    
    virtual void delete_it();
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
    void read_attributes(char *& st, char *& k);
    
    BrowserClass * get_class() const { return cl; }
    void set_class(BrowserClass * t);

    const QValueList<SlotAttr> & get_attributes() const { return attributes; }
    const QValueList<SlotRel> & get_relations() const { return relations; }

    bool exist(BrowserClassInstance * other, RelationData *) const;
    void add(BrowserClassInstance * other, RelationData * rd);
    void replace(BrowserClassInstance * other,
		 RelationData * current, RelationData * future,
		 bool current_isa, bool future_isa);
    void init_other_side();
    
  public slots:
    void check();
    void check_rels();
};

#endif
