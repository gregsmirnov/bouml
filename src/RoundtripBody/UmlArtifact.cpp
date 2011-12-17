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

#include <stdio.h>
#include <qtextstream.h> 

#include "UmlArtifact.h"
#include "UmlPackage.h"
#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlCom.h"
#include "util.h"

void UmlArtifact::roundtrip_cpp() {
  if (! managed) {
    managed = TRUE;
    
    if (stereotype() != "source")
      return;
    
    const QCString hdef = cppHeader();
    const QCString srcdef = cppSource();
    
    if (hdef.isEmpty() && srcdef.isEmpty())
      return;
    
    const QCString & name = UmlArtifact::name();    
    UmlPackage * pack = package();
    QCString h_path = pack->header_path(name);
    QCString src_path = pack->source_path(name);
    
    {
      QCString s;
    
      if (!hdef.isEmpty())
	s = "<i> " + h_path + "</i>";
      
      if (!srcdef.isEmpty()) {	
	if (!hdef.isEmpty())
	  s += " and <i> " + src_path + "</i>";
	else
	  s = "<i> " + src_path + "</i>";
      }
      
      UmlCom::message(name);
      if (verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica>roundtrip body from")
		      + s + "</font><br>");
      else
	set_trace_header(QCString("<font face=helvetica>roundtrip body from")
			 + s + "</font><br>");
    }

    UmlOperation::roundtrip(h_path, cppLanguage);
    UmlOperation::roundtrip(src_path, cppLanguage);
  }
}

void UmlArtifact::roundtrip_java() {
  if (! managed) {
    managed = TRUE;
    
    if (stereotype() != "source")
      return;
    
    const QCString srcdef = javaSource();
    
    if (srcdef.isEmpty())
      return;
    
    const QCString & name = UmlArtifact::name();    
    UmlPackage * pack = package();
    QCString src_path = pack->java_path(name);
    
    {
      QCString s = " <i> " + src_path + "</i>";
      
      UmlCom::message(name);
      if (verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica>roundtrip body from")
		      + s + "</font><br>");
      else
	set_trace_header(QCString("<font face=helvetica>roundtrip body from")
			 + s + "</font><br>");
    }

    UmlOperation::roundtrip(src_path, javaLanguage);
  }
}

void UmlArtifact::roundtrip_php() {
  if (! managed) {
    managed = TRUE;
    
    if (stereotype() != "source")
      return;
    
    const QCString srcdef = phpSource();
    
    if (srcdef.isEmpty())
      return;
    
    const QCString & name = UmlArtifact::name();    
    UmlPackage * pack = package();
    QCString src_path = pack->php_path(name);
    
    {
      QCString s = " <i> " + src_path + "</i>";
      
      UmlCom::message(name);
      if (verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica>roundtrip body from")
		      + s + "</font><br>");
      else
	set_trace_header(QCString("<font face=helvetica>roundtrip body from")
			 + s + "</font><br>");
    }

    UmlOperation::roundtrip(src_path, phpLanguage);
  }
}

void UmlArtifact::roundtrip_python() {
  if (! managed) {
    managed = TRUE;
    
    if (stereotype() != "source")
      return;
    
    const QCString srcdef = pythonSource();
    
    if (srcdef.isEmpty())
      return;
    
    const QCString & name = UmlArtifact::name();    
    UmlPackage * pack = package();
    QCString src_path = pack->python_path(name);
    
    {
      QCString s = " <i> " + src_path + "</i>";
      
      UmlCom::message(name);
      if (verbose())
	UmlCom::trace(QCString("<hr><font face=helvetica>roundtrip body from")
		      + s + "</font><br>");
      else
	set_trace_header(QCString("<font face=helvetica>roundtrip body from")
			 + s + "</font><br>");
    }

    UmlOperation::roundtrip(src_path, pythonLanguage);
  }
}
