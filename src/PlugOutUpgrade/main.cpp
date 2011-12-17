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
#include <qmessagebox.h>

#include "UmlCom.h"
#include "UmlPackage.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;

  QApplication a(argc, argv);
  
  if (UmlCom::connect(QString(argv[1]).toUInt())) {
    bool ok = FALSE;
    
    try {
      UmlCom::trace("<b>Plug-out Upgrade</b> release 1.18<br><hr>");
      if (UmlPackage::getProject()->upgrade())
	UmlCom::trace("<br><hr>Done<br>");
      ok = TRUE;
    }
    catch (...) {
    }
    
    try {
      // socket may be already closed
      if (! ok)
	QMessageBox::critical(0, "Upgrade", 
			      "Error, close the project WITHOUT saving it");


      UmlCom::message("");
      UmlCom::showTrace();
      UmlCom::bye((ok) ? 0 : 1);	// application must not be deleted
    }
    catch (...) {
    }
  }
  
  UmlCom::close();
  return 0;
}
