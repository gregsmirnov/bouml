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

#include "UmlCom.h"
#include "UmlItem.h"
#include "util.h"

// roundtrip_body [-v] c++/java port
  
int main(int argc, char ** argv)
{
  int port_index;
  bool cpp;
  bool java;
  bool php;
  bool python;
  
  if (argc == 3) {
    port_index = 2;
    cpp = (argv[1][0] == 'c');
    java = (argv[1][0] == 'j');
    php = (argv[1][1] == 'h');
    python = (argv[1][1] == 'y');
  }
  else if (argc == 4) {
    set_verbose();
    cpp = (argv[2][0] == 'c');
    java = (argv[2][0] == 'j');
    php = (argv[2][1] == 'h');
    python = (argv[2][1] == 'y');
    port_index = 3;
  }
  else
    return 0;
  
  if (UmlCom::connect(QCString(argv[port_index]).toUInt())) {
    try {
      UmlCom::trace("<b>Roundtrip body</b> release 1.13<br>");
      UmlCom::traceAutoRaise(FALSE);
      if (cpp) 
	UmlCom::targetItem()->roundtrip_cpp();  
      else if (java)
	UmlCom::targetItem()->roundtrip_java();  
      else if (php)
	UmlCom::targetItem()->roundtrip_php();  
      else if (python)
	UmlCom::targetItem()->roundtrip_python(); 
      
      QCString s;
      
      s.sprintf("<hr><font face=helvetica>Roundtrip done : %d errors</font><br>",
		n_errors());
      UmlCom::trace(s);
    
      UmlCom::showTrace();
      UmlCom::message("");
    
      UmlCom::bye(n_errors());
    }
    catch (...) {
    }
  }
  
  UmlCom::close();
  return 0;
}
