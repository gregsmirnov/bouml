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

#include "Statistic.h"
#include "UmlCom.h"

unsigned Statistic::n_classes;
unsigned Statistic::n_attributes;
unsigned Statistic::n_relations;
unsigned Statistic::n_operations;

void Statistic::produce() 
{
  UmlCom::trace(QCString("<font face=helvetica>C++ reverse done</font><br><br>") +
		QCString("<font face=helvetica>	") +
		QCString().setNum(n_classes) + QCString(" classes </font><br>") +
		QCString("<font face=helvetica>	") +
		QCString().setNum(n_attributes) + QCString(" attributes </font><br>") +
		QCString("<font face=helvetica>	") +
		QCString().setNum(n_relations) + " relations </font><br>" +
		QCString("<font face=helvetica>	") +
		QCString().setNum(n_operations) + " operations </font><br><hr><br>");
}
