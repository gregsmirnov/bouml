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
#include <stdlib.h>

#include "UmlCom.h"
#include "UmlItem.h"
#include "UmlPackage.h"
#include "Package.h"
#include "JavaCatWindow.h"
#include "Pixmap.h"
#include "Class.h"

int main(int argc, char ** argv)
{
  if (argc != 2)
      return 0;
  
  if (UmlCom::connect(QCString(argv[1]).toUInt())) {
    try {
      //UmlCom::with_ack(FALSE);
		       
      // warning : release is also in JavaCatWindow
      UmlCom::trace("<b>Java catalog</b> release 2.17<br><hr>");
      UmlCom::traceAutoRaise(FALSE);
      
      UmlItem * item = UmlCom::targetItem();
      
      if (item->kind() != aPackage)
	UmlCom::trace("<font face=helvetica><b>must be applied on a <i>package</i></b></font><br><hr><br>");
      else {
	char * argv = 0;
	int argc = 0;
	
	QApplication * app = new QApplication(argc, &argv);
			 
	app->connect(app, SIGNAL(lastWindowClosed()), SLOT(quit()) );

	init_pixmaps();
	
	JavaCatWindow * w = new JavaCatWindow();
	
	w->resize((QApplication::desktop()->width() * 3)/5,
		  (QApplication::desktop()->height() * 3)/5);
	
	w->show();
	
	Package::init((UmlPackage *) item, app);
	app->exec();
      }
    }
    catch (...) {
    }
    
    try {
      // socket may be already closed
      UmlCom::showTrace();
      UmlCom::bye(0);	// application must not be deleted
    }
    catch (...) {
    }
  }
  
  UmlCom::close();	// application must not be deleted
  return 0;
}
