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

#ifndef PARAMETERDATA_H
#define PARAMETERDATA_H

#include <qtextstream.h>

#include "PinParamData.h"

class ParameterData : public SimpleData, public PinParamData {
  Q_OBJECT
    
  friend class ParameterDialog;
  
  protected:
    MyStr default_value;

  public:
    ParameterData();
    ParameterData(ParameterData * model, BrowserNode * bn);
  
    virtual void do_connect(BrowserClass * c);
    virtual void do_disconnect(BrowserClass * c);
    
    const char * get_default_value() const { return default_value; };

    void edit();
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    virtual void send_cpp_def(ToolCom * com);
    virtual void send_java_def(ToolCom * com);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * &, char * &);
    
  protected slots:
    void on_delete();
};

#endif
