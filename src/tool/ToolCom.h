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

#ifndef TOOLCOM_H
#define TOOLCOM_H

#include <qobject.h>
#include <qlist.h>

#include "BrowserNode.h"
#include "../Tools/ApiCmd.h"

class Socket;
class AType;
class QRect;
class QTimer;

class ToolCom  :public QObject {
  Q_OBJECT
    
  protected:
    static QList<ToolCom> used;
    static QList<ToolCom> unused;
    static int exitvalue;
  
    bool start;
    //bool with_ack;
    bool exit_bouml;
    BrowserNode * target;
    void (*cont)();
    Socket * listen_sock;
    Socket * sock;
    QTimer * timer;
    char * cmd;
    unsigned api_version;
    int id;		// for UmlBaseItem::isToolRunning
    unsigned wanted;
    int already_read;
    char * buffer_in;
    unsigned buffer_in_size;
    char * buffer_out;
    char * p_buffer_out;
    unsigned buffer_out_size;
  
    ToolCom();
    
    void check_size_out(unsigned n);
    const char * read_buffer();

    unsigned bind(unsigned port);
    void close();
    
    static bool is_running(int id);
    
  public:
    unsigned api_format() const { return api_version; }
  
    static int run(const char * cmd, BrowserNode *,
		   bool exit = FALSE, bool clr = TRUE,
		   void (*pf)() = 0);
    friend int exit_value();
  
    void data_received(Socket * who);
    
    unsigned get_unsigned(const char *& p);
    bool get_bool(const char *& p);
    void * get_id(const char *& p);
    UmlCode get_kind(const char *& p);
    const char * get_string(const char *& p);
    bool get_relation_kind(UmlCode & v, const char *& p);
    bool get_visibility(UmlVisibility & v, const char *& p);
    bool get_extended_visibility(UmlVisibility & v, const char *& p);
    bool get_direction(UmlParamDirection & v, const char *& p);
    void get_type(AType & t, const char *& p);
    void skip_type(const char *& p);

    void write_unsigned(unsigned u);
    void write_bool(bool);
    void write_id(BrowserNode * bn, char kind, const char * name);
    void write_id(void * id);
    void write_string(const char * p);
    void write_char(char c);
    void write(QRect r);
    void write_ack(bool);
    void fatal_error(const char * msg);
    
    static void close_all();
    
  signals:
    void closed();
  
  public slots:
    void connexion_timeout();
};

#endif
