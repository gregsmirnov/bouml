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

#ifndef MYIO_H
#define MYIO_H

#include <qdict.h>

class QDir;

char * read_file(QDir & dir, QString fn, BooL & ro);
char * read_token(char *&);
char * read_string(char *&);
bool is_readonly(QDir & dir, QString fn);
bool has_backup_files(QDir & dir);
void delete_backup_files(QDir & dir);
void copy(QDir & fromdir, QDir & todir, QString fn);
void copy_if_needed(QDir & fromdir, QDir & todir, QString fn);
void purge(QDir & dir, QDict<void> & useful);

void set_user_name(int owner, QString s);
QString user_name(int id);

int user_id();
void set_user_id(int, QString);

void remove_crlf(char * s);

#endif
