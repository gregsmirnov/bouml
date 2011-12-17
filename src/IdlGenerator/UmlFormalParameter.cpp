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

#include <qtextstream.h>

#include "UmlFormalParameter.h"
#include "IdlSettings.h"

void UmlFormalParameter::generate(QTextOStream & f, QCString & s, const char *& sep) const {
  f << sep << type() << ' ' << name();
  
  s += sep;
  s += type();
  s += ' ';
  s += name();
      
  QCString dflt = IdlSettings::type(defaultValue().toString());
      
  if (!dflt.isEmpty())
    f << " = " << dflt;
  
  sep = ", ";
}
