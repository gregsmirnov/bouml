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





#include <qfile.h>
#include <qstring.h>
#include <qdir.h>
#include <qmessagebox.h>

static char RemovedChar;
//static int LineNum;

static void skip_whitespace_comment(char * & s)
{
  *s = RemovedChar;
  
  for (;;) {
    switch (*s) {
    case '\n':
      //LineNum += 1;
    case ' ':
    case '\r':
    case '\t':
      s += 1;
      break;
    case '/':
      for (s += 2; *s != '\n'; s += 1)
	if (*s == 0)
	  return;
      //LineNum += 1;
      s += 1;
      break;
    default:
      return;
    }
  }
}

// return the string without the '"'
char * read_string(char * & st)
{
  skip_whitespace_comment(st);
  
  if (*st == 0)
    return 0;

  char * r = st + 1;
  
  for (;;) {
    switch (*++st) {
    case '"':
      *st++ = 0;
      RemovedChar = *st;
      {
	char * p = r;
	char * p2;
	
	while ((p2 = strchr(p, '\\')) != 0) {
	  strcpy(p2, p2 + 1);
	  p = p2 + 1;
	}
	
	p = r;
	
	while ((p2 = strchr(p, '\r')) != 0) {
	  strcpy(p2, p2 + 1);
	  p = p2 + 1;
	}
      }
      return r;
    case '\\':
      st += 1;
      break;
    case '\n':
      //LineNum += 1;
      break;
    }
  }
}

// in case of a string the start '"' is return (not the last one)
char * read_token(char * & st)
{
  skip_whitespace_comment(st);

  if (*st == 0)
    return 0;

  if (*st == '"') {
    RemovedChar = '"';
    return read_string(st) - 1;
  }
  
  char * r = st;
  char c;
  
  do 
    c = *++st;
  while ((c > ' ') && (c != '"'));
  
  RemovedChar = c;
  *st = 0;

  return r;
}

//

static int open_file(QFile & fp, BooL & ro)
{
  QString filename = fp.name();
  
  while (! fp.open(IO_ReadOnly)) {
    if (QMessageBox::critical(0, "Uml",
			      filename + "\ncannot be opened for read, retry ?\n",
			      "yes", "no", 0, 0, 0) != 0)
      return -1;
  }
  
  QFileInfo fi(fp);
  
  ro = !fi.isWritable();
  
  return fi.size();
}

char * read_file(QDir & dir, QString fn, BooL & ro)
{
  QString filename = dir.absFilePath(fn);
  QFile fp(filename);
  int size;
  
  if ((size = open_file(fp, ro)) != -1) {
    char * s = new char[size + 1];
    
    if (fp.readBlock(s, size) == -1) {
      QMessageBox::critical(0, "Error", filename + "cannot be read");
      delete [] s;
      return 0;
    }
    else {
      s[size] = 0;
      //Linenum = 1;
      RemovedChar = *s;
    
      if (fn[0].isNumber() && ((fn.toInt() & 127) == 0))
	ro = TRUE;
 
      return s;
    }
  }
  else
    return 0;
}

//

QString User[128];

void set_user_name(int id, QString s)
{
  User[id] = s;
}

QString user_name(int id)
{
  return User[id];
}

//

static int uid;

int user_id()
{
  return uid;
}

void set_user_id(int id, QString s)
{
  User[id] = s;
  uid = id;
}

//

void remove_crlf(char * s)
{
  int len = strlen(s);
  
  if (len != 0) {
    if (s[len - 1] == '\n')
      s[--len] = 0;
    
    if ((len != 0) && (s[len - 1] == '\r'))
      s[len - 1] = 0;
  }
}
