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

#include "UmlCom.h"
#include "UmlItem.h"
#include "CppSettings.h"
#include "UmlPackage.h"
#include "Package.h"
#include "Lex.h"
#include "Statistic.h"

int main(int argc, char ** argv)
{
  if (argc != 2)
      return 0;
  
  if (UmlCom::connect(QCString(argv[1]).toUInt())) {
    try {
      //UmlCom::with_ack(FALSE);
      UmlCom::trace("<b>C++ reverse</b> release 2.15<br>");
      UmlCom::traceAutoRaise(FALSE);
      
      UmlItem * item = UmlCom::targetItem();
      
      if (item->kind() != aPackage)
	UmlCom::trace("<font face=helvetica><b>must be applied on a <i>package</i></b></font><br><hr><br>");
      else {
	char * argv = 0;
	int argc = 0;
	QApplication * app = new QApplication(argc, &argv);
				 
	Package::init((UmlPackage *) item, app);
	
	QCString f;
	
	if (UmlPackage::getProject()->propertyValue("#file", f))
	  Lex::defines(f);
	
	// add c++ catalog like java ?
	
	int n;
	
	Package::scan_dirs(n);
	
	if (n != 0) {
	  CppSettings::set_UseDefaults(TRUE);
	  Package::send_dirs(n, TRUE);
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
  
  UmlCom::close();
  return 0;
}
