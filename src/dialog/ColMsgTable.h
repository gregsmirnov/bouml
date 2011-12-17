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

#ifndef COLMSGTABLE_H
#define COLMSGTABLE_H

#include <qptrdict.h> 

#include "MyTable.h"

class ColMsg;
class ColMsgList;

class ColDiagramView;

class ColMsgTable : public MyTable {
  Q_OBJECT
    
  protected:
    bool rec;
    ColDiagramView * view;
    ColMsgList & msgs;
    QValueList<ColMsg *> flat_msg_list;
    
    void refresh();
    void refresh(ColMsgList & m);
    void edit_msg(int row);
    void change_ranks(int row, int col);
    
#ifdef NEW_METHOD
    void save_list(ColMsgList & l, QPtrDict<ColMsgList> & saved);
#endif
    
  public:
    ColMsgTable(QWidget * parent, ColDiagramView * v, ColMsgList & m);
 
  protected slots:
    virtual void button_pressed(int row, int col, int button, const QPoint & mousePos);
};

#endif
