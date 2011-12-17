// *************************************************************************cr();
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

#ifndef EXPANSIONREGIONDATA_H
#define EXPANSIONREGIONDATA_H

#include "SimpleData.h"
#include "UmlEnum.h"

class QTextStream;

class BrowserExpansionRegion;
class ExpansionRegionDialog;

class ExpansionRegionData : public SimpleData {
  friend class ExpansionRegionDialog;
    
  protected:
    bool must_isolate;
    UmlExpansionKind mode;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
    
  public:
    ExpansionRegionData();
    ExpansionRegionData(ExpansionRegionData * model, BrowserNode * br);
    virtual ~ExpansionRegionData();
    
    bool get_is_must_isolate() const { return must_isolate; }
    UmlExpansionKind get_mode() const { return mode; }
    
    void edit();
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
};
  
#endif
