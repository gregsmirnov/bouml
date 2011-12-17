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

#ifndef EXTRAMEMBERDATA_H
#define EXTRAMEMBERDATA_H

#include "ClassMemberData.h"

class BrowserExtraMember;

class ExtraMemberData : public BasicData {
  Q_OBJECT
    
  friend class ExtraMemberDialog;
  
  protected:
    bool is_deleted;
    
    // C++
    bool cpp_inline;
    MyStr cpp_decl;
    MyStr cpp_def;
    
    // Java
    MyStr java_decl;
    
    // Php
    MyStr php_decl;
    
    // Python
    MyStr python_decl;
    
    // Idl
    MyStr idl_decl;
    
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    virtual void send_php_def(ToolCom * com);
    virtual void send_python_def(ToolCom * com);
    virtual void send_idl_def(ToolCom * com);
    
  public:
    ExtraMemberData();
    ExtraMemberData(const ExtraMemberData * model, BrowserNode *);
    virtual ~ExtraMemberData();
    
    virtual bool deletedp() const;
    virtual void set_deletedp(bool y);
        
    virtual QString definition(bool full, bool with_kind) const;
    
    const char * get_cpp_decl() const { return cpp_decl; };
    const char * get_cpp_def() const { return cpp_def; };
    bool get_cpp_inline() const { return cpp_inline; };
            
    const char * get_java_decl() const { return java_decl; };
            
    const char * get_php_decl() const { return php_decl; };
    
    const char * get_python_decl() const { return python_decl; };
    
    const char * get_idl_decl() const { return idl_decl; };
    
    void edit();
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
};

#endif
