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

int main(int argc, char ** argv)
{
  int port_index;
  
  if (argc == 2)
    port_index = 1;
  else if (argc == 3) {
    if (argv[1][1] == 'v')
      set_verbose();
    else
      set_preserve();
    port_index = 2;
  }
  else if (argc == 4) {
    set_verbose();
    set_preserve();
    port_index = 3;
  }
  else
    return 0;
  
  if (UmlCom::connect(QCString(argv[port_index]).toUInt())) {
    try {
      UmlCom::trace("<b>Java generator</b> release 2.22<br>");
      UmlCom::traceAutoRaise(FALSE);
      
      UmlCom::targetItem()->generate();
      
      QCString s;
      
      s.sprintf("<hr><font face=helvetica>Generation done : %d warnings, %d errors</font><br>",
		n_warnings(), n_errors());
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
