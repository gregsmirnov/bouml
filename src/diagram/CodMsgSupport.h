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

#ifndef CODMSGSUPPORT_H
#define CODMSGSUPPORT_H

#include <qlist.h>

#include "Settings.h"
#include "ColMsg.h"

class QString;
class QTextStream;

class CodObjCanvas;
class BrowserNode;

class CodMsgSupport {
  protected:
    ColMsgList msgs;
    CollaborationDiagramSettings settings;
  
  public:
    CodMsgSupport(){};
    virtual ~CodMsgSupport();
    
    ColMsgList & get_msgs() { return msgs; };
    
    void delete_it(ColMsgList & top);
    virtual void remove_it(ColMsg * msg) = 0;
    virtual void save(QTextStream & st, bool ref, QString & warning) const = 0;
    virtual void get_from_to(CodObjCanvas *& from, CodObjCanvas *& to,
			     bool forward) = 0;
    virtual void update_msgs() = 0;
    virtual bool copyable() const = 0;
    
    bool supports(BrowserNode *);
};

#endif
