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

#include <qdir.h>

#include "UmlPackage.h"
#include "UmlCom.h"
#include "IdlSettings.h"
#include "util.h"

UmlPackage::UmlPackage(void * id, const QCString & n)
    : UmlBasePackage(id, n) {
  read = FALSE;
}

static void create_directory(QCString s)
{
  int index = 0;
  char sep = QDir::separator();
  
  if (sep != '/') {
    while ((index = s.find(sep, index)) != -1)
      s.replace(index++, 1, "/");
  }
  
  s = QDir::cleanDirPath(s) + "/";
  index = s.find("/");
  
  int index2;
  
  while ((index2 = s.find("/", index + 1)) != -1) {
    QCString s2 = s.left(index2);
    QDir sd(s2);
    
    if (!sd.exists()) {
      if (!sd.mkdir(s2)) {
	UmlCom::trace(QCString("<font color=\"red\"><b> cannot create directory <i>")
		      + s2 + "</i></b></font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("UmlPackage::file_path");
      }
    }
    index = index2;
  }
}

static bool RootDirRead;
static QCString RootDir;

QCString UmlPackage::path(const QCString & f) {
  if (!read) {
    dir = idlDir();
    
    if (! RootDirRead) {
      RootDirRead = TRUE;
      RootDir = IdlSettings::rootDir();

      if (!RootDir.isEmpty() && // empty -> error
	  QDir::isRelativePath(RootDir)) {
	QFileInfo f(getProject()->supportFile());
	QDir d(f.dirPath());

	RootDir = d.filePath(RootDir);
      }
    }

    QDir d_root(RootDir);
    
    if (dir.isEmpty())
      dir = RootDir;
    else if (QDir::isRelativePath(dir))
      dir = d_root.filePath(dir);

    if (dir.isEmpty()) {
      UmlCom::trace(QCString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
			    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
			    "or edit the package (tab 'Idl')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::file_path");
    }
    
    if (QDir::isRelativePath(dir)) {
      UmlCom::trace(QCString("<font color=\"red\"><b><i>")
		    + name() + "</i>'s source path <i>(" + dir
		    + "</i>) is not absolute, edit the <i> generation settings</i> "
		    "(tab 'directory'), or edit the package (tab 'Idl')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::file_path");
    }

    read = TRUE;
  }
  
  QDir d(dir);
  
  if (! d.exists())
    create_directory(dir);	// don't return on error
  
  return QCString(d.filePath(f)) + QCString(".") + 
    IdlSettings::sourceExtension();
}

QCString UmlPackage::text_path(const QCString & f) {
  QCString r = path(f);
  
  return r.left(r.length() - 1 - IdlSettings::sourceExtension().length());
}

void UmlPackage::generate() {
  QVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->generate();
}

