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

#ifndef PSEUDOSTATEDATA_H
#define PSEUDOSTATEDATA_H

#include <qptrdict.h>

#include "SimpleData.h"

class BrowserPseudoState;

class PseudoStateData : public SimpleData {
  Q_OBJECT
    
  friend class PseudoStateDialog;
  
  protected:
    BrowserPseudoState * reference;
    
    virtual void send_uml_def(ToolCom * com, BrowserNode * bn,
			      const QString & comment);
  public:
    PseudoStateData();
    PseudoStateData(PseudoStateData * model, BrowserNode *);
    virtual ~PseudoStateData();
    
    BrowserPseudoState * get_reference() const { return reference; }
    void set_reference(BrowserPseudoState *);
    
    void edit();
    
    virtual bool tool_cmd(ToolCom * com, const char * args,
			  BrowserNode * bn, const QString & comment);
    
    void save(QTextStream &, QString & warning) const;
    void read(char * & st, char * & k);
    
  protected slots:
    void on_delete();
};

#endif
