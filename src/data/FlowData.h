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

#ifndef FLOWDATA_H
#define FLOWDATA_H

#include "SimpleData.h"
#include "InfoData.h"
#include "UmlEnum.h"

class QTextStream;

class BrowserFlow;
class BrowserNode;
    
struct FlowDef {
  SharedStr weight;
  MyStr guard;
  InfoData sel_trans;
  
  QString str(bool horiz) const;
  void save(QTextStream & st, QCString lang) const;
  void read(char * & st, char * & k, QCString lang);
  
  void send_def(ToolCom * com);
};
    
class FlowData : public SimpleData {
  Q_OBJECT

  friend class FlowDialog;
  
  protected:
    BrowserNode * end;
    FlowDef uml;
    FlowDef cpp;
    FlowDef java;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    
  public:
    FlowData();
    FlowData(const BrowserFlow * model, BrowserFlow * r);
    virtual ~FlowData();
        
    virtual void delete_it();
    
    void set_start_end(BrowserFlow * s, BrowserNode * e);
    void edit();
    BrowserNode * get_start_node() const;
    BrowserNode * get_end_node() const { return end; }
    BrowserFlow * get_start() const { 
      return (BrowserFlow *) browser_node;
    }
    QString str(bool horiz, DrawingLanguage) const;
    QString selection(DrawingLanguage) const;
    QString transformation(DrawingLanguage) const;
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    static FlowData * read(char * &, char * & k);
    
  protected slots:
    void end_deleted();
};

#endif
