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
#include <qtextstream.h> 

#include "UmlPackage.h"
#include "UmlCom.h"
#include "PythonSettings.h"
#include "util.h"

UmlPackage::UmlPackage(void * id, const QCString & n)
    : UmlBasePackage(id, n) {
  dir.read = FALSE;
}

static bool RootDirRead;
static QCString RootDir;

QCString UmlPackage::file_path(const QCString & f) {
  if (!dir.read) {
    dir.file = pythonDir();
    
    if (! RootDirRead) {
      RootDirRead = TRUE;
      RootDir = PythonSettings::rootDir();

      if (!RootDir.isEmpty() && // empty -> error
	  QDir::isRelativePath(RootDir)) {
	QFileInfo f(getProject()->supportFile());
	QDir d(f.dirPath());

	RootDir = d.filePath(RootDir);
      }
    }

    QDir d_root(RootDir);
    
    if (dir.file.isEmpty())
      dir.file = RootDir;
    else if (QDir::isRelativePath(dir.file))
      dir.file = d_root.filePath(dir.file);
   
    if (dir.file.isEmpty()) {
      UmlCom::trace(QCString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
			    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
			    "or edit the package (tab 'Python')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::file_path");
    }
    
    dir.read = TRUE;
  }
  
  QDir d(dir.file);
  
  if (! d.exists()) {
    // create directory including the intermediates
    QCString s = dir.file;
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
  
  return QCString(d.filePath(f)) + QCString(".") + 
    PythonSettings::sourceExtension();
}

QCString UmlPackage::text_path(const QCString & f) {
  QCString r = file_path(f);
  
  return r.left(r.length() - 1 - PythonSettings::sourceExtension().length());
}

void UmlPackage::generate() {
  QVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->generate();
}

UmlPackage * UmlPackage::package() {
  return this;
}

