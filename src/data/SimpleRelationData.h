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

#ifndef SIMPLERELATIONDATA_H
#define SIMPLERELATIONDATA_H

#include "SimpleData.h"
#include "UmlEnum.h"

class QTextStream;

class BrowserSimpleRelation;
class BrowserNode;

class SimpleRelationData : public SimpleData {
  Q_OBJECT

  friend class SimpleRelationDialog;
  
  protected:
    UmlCode type;
    BrowserNode * end;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    
  public:
    SimpleRelationData(UmlCode e);
    SimpleRelationData(const BrowserSimpleRelation * model, BrowserSimpleRelation * r);
    virtual ~SimpleRelationData();
        
    virtual void delete_it();
    
    void set_start_end(BrowserSimpleRelation * s, BrowserNode * e);
    void edit();
    BrowserNode * get_start_node() const;
    BrowserNode * get_end_node() const { return end; }
    BrowserSimpleRelation * get_start() const { 
      return (BrowserSimpleRelation *) browser_node;
    }
    
    UmlCode get_type() const { return type; }
    void set_type(UmlCode e);
    
    virtual QString definition(bool full, bool with_kind) const;    
    
    void save(QTextStream &, QString & warning) const;
    static SimpleRelationData * read(char * &);
    
  protected slots:
    void end_deleted();
};

#endif
