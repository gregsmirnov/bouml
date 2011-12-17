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

#include <qapplication.h>
#include <qfiledialog.h> 
#include <stdlib.h>
#include <qfile.h>
#include <qdir.h>
#include <qdatastream.h> 
#include <qtextstream.h>

#include "Class.h"
#include "UmlClass.h"
#include "UmlCom.h"
#include "UmlItem.h"
#include "UmlPackage.h"
#include "Package.h"
#include "JavaSettings.h"
#include "Statistic.h"

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

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;
  
  if (UmlCom::connect(QCString(argv[1]).toUInt())) {
    try {
      //UmlCom::with_ack(FALSE);
      UmlCom::trace("<b>Java reverse</b> release 2.18<br><hr>");
      UmlCom::traceAutoRaise(FALSE);
      
      UmlItem * item = UmlCom::targetItem();
      
      if (item->kind() != aPackage)
	UmlCom::trace("<font face=helvetica><b>must be applied on a <i>package</i></b></font><br><hr><br>");
      else {
	char * argv = 0;
	int argc = 0;
				 
	QApplication * app = new QApplication(argc, &argv);
			 
	Package::init((UmlPackage *) item, app);
	
	QString here = QDir::currentDirPath();
	QString path;
	// note : QFile fp(QDir::home().absFilePath(".boumlcat")) doesn't work
	// if the path contains non latin1 characters, for instance cyrillic !
	QString s = QDir::home().absFilePath(".boumlcat");
	FILE * fp = fopen((const char *) s, "r");
  














	
	if (fp != 0) {
	  char line[512];
	  
	  if (fgets(line, sizeof(line) - 1, fp) != 0) {
	    remove_crlf(line);
	    path = line;
	  }
	  
	  fclose(fp);
	}
	
	while (!(path = 
		 QFileDialog::getOpenFileName(path, "*.cat",
					      0, 0,
					      "select a java catalog file to read it, or cancel"))
	       .isEmpty()) {
	  QFile f(path);
	  
	  if (f.open(IO_ReadOnly)) {
	    if ((fp = fopen((const char *) s, "w")) != 0) {
	      fwrite((const char *) path, 1, path.length(), fp);
	      fputc('\n', fp);
	      fclose(fp);
	    }
	    
	    QDataStream dt(&f);
	    
	    Package::get_root()->restore_children(dt);
	  }
	}
	
	QDir::setCurrent(here);
	int n;
	
	Package * p = Package::scan_dir(n);
	
	if (p != 0) {
	  JavaSettings::set_UseDefaults(TRUE);
	  
	  p->send_dir(n);
	  Statistic::produce();
	}
      }
    }
    catch (...) {
    }
    
    try {
      // socket may be already closed
      UmlCom::message("");
      UmlCom::showTrace();
      UmlCom::bye(0);	// application must not be deleted
    }
    catch (...) {
    }
  }
  
  UmlCom::close();	// application must not be deleted
  return 0;
}
