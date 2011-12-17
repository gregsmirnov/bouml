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

#ifndef FORMALPARAMDATA_H
#define FORMALPARAMDATA_H

#include <qobject.h>

#include "mystr.h"

#include "AType.h"

class FormalParamData : public QObject {
  Q_OBJECT
    
  protected:
    MyStr name;
    SharedStr type;		// "class" ..., C++
    AType default_value;	// C++
    AType extends;		// Java

  public:
    FormalParamData() {};
    FormalParamData(const FormalParamData& p);
    FormalParamData& operator=(const FormalParamData&);
  
    const char * get_name() const { return name; };
    void set_name(const QString & s) { name = s; };
    void set_name(const char * s) { name = s; };
    
    const char * get_type() const { return type; };
    void set_type(const char * s) { type = s; };
    void set_type(const QString & s) { type = s; };
    
    const AType & get_default_value() const { return default_value; };
    void set_default_value(BrowserClass * t, const QString & e);
    
    const AType & get_extends() const { return extends; };
    void set_extends(BrowserClass * t, const QString & e);
    
    void send_uml_def(ToolCom * com);
    void read(ToolCom * com, const char *& args);
    static void skip(ToolCom * com, const char *& args);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * &);
    
  protected slots:
    void on_delete_default();
    void on_delete_extends();
};

#endif
