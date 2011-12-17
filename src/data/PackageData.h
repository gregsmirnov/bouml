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

#ifndef PACKAGEDATA_H
#define PACKAGEDATA_H

#include "SimpleData.h"

class QTextStream;

class BrowserPackage;
class PackageDialog;

class PackageData : public SimpleData {
  friend class PackageDialog;
    
  protected:
    MyStr cpp_src_dir;
    MyStr cpp_h_dir;
    MyStr java_dir;
    MyStr php_dir;
    MyStr python_dir;
    MyStr idl_dir;
    
    MyStr cpp_namespace;
    MyStr java_package;
    MyStr php_namespace;
    MyStr python_package;
    MyStr idl_module;
    
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    virtual void send_php_def(ToolCom * com);
    virtual void send_python_def(ToolCom * com);
    virtual void send_idl_def(ToolCom * com);
    
  public:
    PackageData();
    PackageData(PackageData * model);
    virtual ~PackageData();
    
    void edit();
    
    const MyStr & get_cpp_src_dir() const { return cpp_src_dir; };
    const MyStr & get_cpp_h_dir() const { return cpp_h_dir; };
    const MyStr & get_java_dir() const { return java_dir; };
    const MyStr & get_php_dir() const { return php_dir; };
    const MyStr & get_python_dir() const { return python_dir; };
    const MyStr & get_idl_dir() const { return idl_dir; };
    
    const MyStr & get_cpp_namespace() const { return cpp_namespace; };
    const MyStr & get_java_package() const { return java_package; };
    const MyStr & get_php_namespace() const { return php_namespace; };
    const MyStr & get_python_package() const { return python_package; };
    const MyStr & get_idl_module() const { return idl_module; };
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
};
  
#endif
