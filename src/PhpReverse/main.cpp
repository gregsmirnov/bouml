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
#include "PhpSettings.h"
#include "Statistic.h"

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;
  
  if (UmlCom::connect(QCString(argv[1]).toUInt())) {
    try {
      //UmlCom::with_ack(FALSE);
      
      UmlCom::trace("<b>Php reverse</b> release 1.8.1<br><hr>");
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
	
	/*



	QString path;
	QFile fp(QDir::home().absFilePath(".boumlcat"));
	
	if (fp.open(IO_ReadOnly)) {
	  QTextStream ts(&fp);
	  
	  path = ts.readLine();
	  fp.close();
	}
	
	while (!(path = 
		 QFileDialog::getOpenFileName(path, "*.cat",
					      0, 0,
					      "select a php catalog file to read it, or cancel"))
	       .isEmpty()) {
	  QFile f(path);
	  
	  if (f.open(IO_ReadOnly)) {
	    if (fp.open(IO_WriteOnly)) {
	      QTextStream ts(&fp);
	      
	      ts.writeRawBytes((const char *) path, path.length());
	      ts.writeRawBytes("\n", 1);
	    }
	    
	    QDataStream dt(&f);
	    
	    Package::get_root()->restore_children(dt);
	  }
	}
	*/
	
	QDir::setCurrent(here);
	
	Package * p = Package::scan_dir();
	
	if (p != 0) {
	  PhpSettings::set_UseDefaults(TRUE);
	  
	  p->send_dir(TRUE);
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
