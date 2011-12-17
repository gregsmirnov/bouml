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

#include "UmlAttribute.h"

#include "UmlClass.h"
#include "UmlCom.h"
#include "util.h"


UmlAttribute * UmlAttribute::java2Php(UmlClass * php, UmlClass * java,
				      const char * javaname,
				      const char * phpname)
{
  if (phpname == 0)
    phpname = javaname;
  
  UmlAttribute * from = java->get_attribute(javaname);
  
  if (from == 0) {
    QCString err = QCString("cannot find attribute '") + 
      javaname + QCString("' in class '") + java->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlAttribute * to = UmlBaseAttribute::create(php, phpname);
  
  if (to == 0) {
    QCString err = QCString("cannot create attribute '") + 
      phpname + QCString("' in class '") + php->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlCom::trace("add attribute " + php->name() + "::" + phpname + "<br>\n");

  to->set_Type(from->type());
  to->set_isClassMember(from->isClassMember());
  to->set_Visibility(from->visibility());
  to->set_CppVisibility(from->cppVisibility());
  to->set_CppDecl(::java2Php(from->cppDecl()));
  to->set_JavaDecl(from->javaDecl());
  
  return to;
}

UmlAttribute * UmlAttribute::java2Python(UmlClass * python, UmlClass * java,
					 const char * javaname,
					 const char * pythonname)
{
  if (pythonname == 0)
    pythonname = javaname;
  
  UmlAttribute * from = java->get_attribute(javaname);
  
  if (from == 0) {
    QCString err = QCString("cannot find attribute '") + 
      javaname + QCString("' in class '") + java->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlAttribute * to = UmlBaseAttribute::create(python, pythonname);
  
  if (to == 0) {
    QCString err = QCString("cannot create attribute '") + 
      pythonname + QCString("' in class '") + python->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlCom::trace("add attribute " + python->name() + "::" + pythonname + "<br>\n");

  to->set_Type(from->type());
  to->set_isClassMember(from->isClassMember());
  to->set_Visibility(from->visibility());
  to->set_CppVisibility(from->cppVisibility());
  to->set_CppDecl(::java2Python(from->cppDecl()));
  to->set_JavaDecl(from->javaDecl());
  
  return to;
}

UmlAttribute * UmlAttribute::cpp2Python(UmlClass * python, UmlClass * cpp,
					const char * cppname,
					const char * pythonname)
{
  if (pythonname == 0)
    pythonname = cppname;
  
  UmlAttribute * from = cpp->get_attribute(cppname);
  
  if (from == 0) {
    QCString err = QCString("cannot find attribute '") + 
      cppname + QCString("' in class '") + cpp->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlAttribute * to = UmlBaseAttribute::create(python, pythonname);
  
  if (to == 0) {
    QCString err = QCString("cannot create attribute '") + 
      pythonname + QCString("' in class '") + python->name()
	+ QCString("'<br>\n");
    UmlCom::trace(err);
    throw 0;
  }
  
  UmlCom::trace("add attribute " + python->name() + "::" + pythonname + "<br>\n");

  to->set_Type(from->type());
  to->set_isClassMember(from->isClassMember());
  to->set_Visibility(from->visibility());
  to->set_CppVisibility(from->cppVisibility());
  to->set_CppDecl(::cpp2Python(from->cppDecl()));
  to->set_JavaDecl(from->javaDecl());
  
  return to;
}
