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

#ifndef STATEDATA_H
#define STATEDATA_H

#include "SimpleData.h"
#include "UmlEnum.h"

class QTextStream;

class BrowserState;
class StateDialog;
class BrowserOperation;

struct StateBehavior {
  MyStr on_entry;
  MyStr on_exit;
  MyStr do_activity;
  
  void save(QTextStream &, const char * lang) const;
  void read(char * & st, char * & k, const char * lang);
  
  void send_def(ToolCom * com);
};

class StateData : public SimpleData {
  Q_OBJECT
    
  friend class StateDialog;
    
  protected:
    bool is_active;
    StateBehavior uml;
    StateBehavior cpp;
    StateBehavior java;
    BrowserOperation * specification;
    BrowserState * reference;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    
  public:
    StateData();
    StateData(StateData * model, BrowserNode * br);
    virtual ~StateData();
    
    const StateBehavior & get_behavior(DrawingLanguage) const;
    bool get_is_active() const { return is_active; }
    BrowserOperation * get_specification() const { return specification; }
    void set_specification(BrowserOperation *);
    BrowserState * get_reference() const { return reference; }
    void set_reference(BrowserState *);
    
    void edit();
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
    
  protected slots:
    void on_delete();
};
  
#endif
