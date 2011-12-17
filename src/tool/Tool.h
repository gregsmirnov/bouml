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

#ifndef TOOL_H
#define TOOL_H

#include <qstring.h>

#include "UmlEnum.h"

class QPopupMenu;
class QObject;
class QTextStream;

class ATool {
  public:
    QString display;
    QString cmd;
    bool applicable[UmlCodeSup];
    
    ATool();
};

class Tool {
  friend class ToolDialog;
  friend class ToolTable;
  protected:
    static unsigned ntools;
    static ATool * tools;
    static bool already_read;
    
    static void set_ntools(unsigned n);
    static void read(char * & st, char * & k, bool new_format);
    
  public:
    static bool menu_insert(QPopupMenu * tool, UmlCode target, 
			    const QObject * receiver, const char * member);
    static bool menu_insert(QPopupMenu * tool, UmlCode target, int first_id);
    static void shortcut(QString s, int & choice, UmlCode target, int first_id);
    static QStringList all_display();
    static const char * command(int rank);
    static const char * command(const char *);
    
    static void save();
    static bool read(const char * = 0);
    static void read(char * & st, char * & k);
    static bool import();
    static void add();
    
    static void init();
    static void defaults();
};

void toggle_verbose_generation();
bool verbose_generation();

void toggle_preserve_bodies();
bool preserve_bodies();

void toggle_add_operation_profile();
bool add_operation_profile();

#endif
