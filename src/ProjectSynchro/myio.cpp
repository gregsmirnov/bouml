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

#include "myio.h"
#include "SynchroWindow.h"

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

bool is_readonly(QDir & dir, QString fn)
{
  QFileInfo fi(dir.absFilePath(fn));
  
  return fi.exists() && !fi.isWritable();
}

bool has_backup_files(QDir & dir)
{
  const QFileInfoList * l = dir.entryInfoList("*.bak");
  
  return (l != 0) && !l->isEmpty();
}

void abort(QString s)
{
  s += "\n\n\
to nullify the synchronization for this project you\n\
must rename all files xxx.bak to xxx. Warning, some\n\
extra files corresponding to the new version may be\n\
present, resynchronize after renaming the bak files";

  QMessageBox::critical(0, "Project synchro",
			"Synchronization aborted :\n\n" + s);
  SynchroWindow::abort();
}

void copy(QDir & fromdir, QDir & todir, QString fn)
{
  QString to = todir.absFilePath(fn);
  QFileInfo fi(to);
  
  if (fi.exists() && !todir.rename(to, to + ".bak"))
    abort(to + " can't be renamed " + to + ".bak");

  QString from = fromdir.absFilePath(fn);
  FILE * fpin = fopen((const char *) from, "rb");
  
  if (fpin != 0) {
    FILE * fpout = fopen((const char *) to, "wb");
    
    static char buff[256];
    unsigned n;
    
    while ((n = (unsigned) fread(buff, 1, sizeof(buff), fpin)) != 0)
      if ((unsigned) fwrite(buff, 1, n, fpout) != n)
	abort("error writing in " + to);
    
    fclose(fpout);
    fclose(fpin);
  }
}

void copy_if_needed(QDir & fromdir, QDir & todir, QString fn)
{
  QString to = todir.absFilePath(fn);
  QFileInfo ti(to);
  
  if (!ti.exists())
    // to rename from
    copy(fromdir, todir, fn);
  else {
    QString from = fromdir.absFilePath(fn);
    FILE * fpfrom = fopen((const char *) from, "rb");
  
    if (fpfrom == 0)
      // to create from
      copy(fromdir, todir, fn);
    else {
      FILE * fpto = fopen((const char *) to, "rb");
      
      if (fpto == 0) {
	// supose copy needed
	fclose(fpfrom);
	copy(fromdir, todir, fn);
      }
      else {
	bool docopy = FALSE;
	int c;
	
	while((c = fgetc(fpfrom)) != EOF) {
	  if (c != fgetc(fpto)) {
	    docopy = TRUE;
	    break;
	  }
	}
	
	if (! docopy)
	  docopy = (fgetc(fpto) != EOF);
	
	fclose(fpfrom);
	fclose(fpto);
	  
	if (docopy)
	  copy(fromdir, todir, fn);
      }
    }
  }
}

void purge(QDir & dir, QDict<void> & useful)
{
  const QFileInfoList * l;
    
  // remove bak and useless diagrams and bodies
  if ((l = dir.entryInfoList("*.diagram *.bodies *.bak")) != 0) {
    QListIterator<QFileInfo> it(*l);
    QFileInfo *fi;
    
    while ((fi = it.current()) != 0) {
      if ((useful.find(fi->fileName()) == 0) &&
	  ! QFile::remove(fi->absFilePath()))
	QMessageBox::critical(0, "Project synchro",
			      "can't remove " + fi->absFilePath() +
			      ", do it yourself");
      ++it;
    }
  }
  
  // remove deleted packages
  if ((l = dir.entryInfoList("*")) != 0) {
    QListIterator<QFileInfo> it(*l);
    QFileInfo *fi;
    
    while ((fi = it.current()) != 0) {
      QString s = fi->fileName();
      const char * p = s;
      
      while ((*p >= '0') && (*p <= '9'))
	p += 1;
      
      if ((*p == 0) &&
	  (useful.find(s) == 0) &&
	  ! QFile::remove(fi->absFilePath()))
	QMessageBox::critical(0, "Project synchro",
			      "can't remove " + fi->absFilePath() +
			      ", do it yourself");
      
      ++it;
    }
  }
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

