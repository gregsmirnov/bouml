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

#ifndef UMLOPERATION_H
#define UMLOPERATION_H

#include "UmlBaseOperation.h"
#include "UmlClass.h"

class Class;

class UmlOperation : public UmlBaseOperation {
  private:
    static bool read_param(Class * container, unsigned rank,
			   const QValueList<FormalParameterList> & tmplts,
			   UmlParameter & param, QCString & decl, bool bypass);
  
  public:
    UmlOperation(void * id, const QCString & n)
      : UmlBaseOperation(id, n) {};
	
    static bool new_one(Class * container, const QCString & name,
			const QValueList<FormalParameterList> & tmplts,
			const QCString & oper_templ,
			UmlTypeSpec & type, QCString str_actuals,
			UmlClass * first_actual_class, QCString type_def,
			aVisibility visibility,
			bool finalp, bool abstractp, bool staticp,
			bool nativep, bool strictfp, bool synchronizedp,
			const QCString & array,	QCString comment,
			QCString description, QCString annotation
#ifdef ROUNDTRIP
			, bool roundtrip, QList<UmlItem> & expected_order
#endif
			);

    static void skip_body(int level = 0);
    static char skip_expr(int level = 0);

#ifdef ROUNDTRIP
  private:
    static UmlOperation * already_exist_from_id(Class * container, QCString & body);
    static UmlOperation * already_exist(Class * container, const QCString & name,
					QValueList<UmlParameter> & params);
#endif
};

#endif
