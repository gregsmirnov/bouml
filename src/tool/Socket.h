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

#ifndef SOCKET_H
#define SOCKET_H

#include <qsocketdevice.h> 
#include <qsocketnotifier.h> 

class ToolCom;

class Socket : public QObject, public QSocketDevice {
  Q_OBJECT
    
  protected:
    ToolCom * com;
    QSocketNotifier * notifier;
    bool on_error;
  
  public:
    Socket(ToolCom * c);
    Socket(ToolCom * c, int s);
    ~Socket();
    
    bool write_block(char * data, unsigned int len);
    
  protected slots:
    void data_received();
};
  
#endif
